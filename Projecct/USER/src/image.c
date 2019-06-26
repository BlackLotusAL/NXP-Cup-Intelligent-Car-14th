#include "headfile.h"
#include "image.h"

int row, line;										//定义列和行数
int left_Max = 0, right_Max = 187;					//左、右边界识别阈值（可调整）
int bin_threshold;									//二值化阈值
int flag_single_0;									//用于第一行的单边界判断
int left_search,			right_search;			//无边界时寻找
int left_before,			right_before;			//无边界时寻找下一行的参考值
int left_none_start,		right_none_start;
int flag_none_left_start,	flag_none_right_start;
int flag_single_left = 0,	flag_single_right = 0;	//左、右侧单边界判断
int flag_try_left = 0, 		flag_try_right = 0;		//用于检测某行是否还存在道路边界
int left[120], right[120], middle[120], valid[120];	//左、右道路边界，中线
uint8 picture[120][188];
uint16 picture_rgb[120][188];

float image_error;

void Image_Handle()//图像处理总函数
{
	Image_Transfer();
	Image_Threshold();
	Image_Bin();
    First_Line_Find();
    for(line=118;line>0;line--)//每循环一次得到一个Left[line]和一个Right[line]循环结束则边界赋值结束
    {
        Find_Next_Right();
        Find_Next_Left();
        if((right[line]==0||right[line]==187)&&(left[line]==0||left[line]==187))
        {
            valid[line]=1;
            continue;
        }
        middle[line]=(right[line]+left[line])/2;
    }
    
    Image_RGB();

	image_error = (float)middle[110]*0.3f+(float)middle[90]*0.3f+(float)middle[70]*0.2f+(float)middle[60]*0.1f+(float)middle[50]*0.1f-94.0f;
}

void Image_Transfer()
{
	for(int i = 0; i < 120; i++)
    {
        for(int j = 0; j < 188; j++)
        {
            picture[i][j] = image[i][j];
        }
    }
}

void Image_Threshold()//计算阈值
{
	long long int sum = 0;
	for(int i = 0; i < 120; i = i + 5)
    {
        for(int j = 0; j < 188; j = j + 4)
        {
            sum += picture[i][j];
        }
    }
    bin_threshold = sum / 1128;
}

void Image_Bin()//二值化
{
	for(int i = 0; i < 120; i++)
	{
		for(int j = 0; j < 188; j++)
        {
            if(picture[i][j] > bin_threshold)
			{
				picture[i][j] = 0xff;
			}
            else
			{
				picture[i][j] = 0x00;
			}
        }
    }
}


//寻找第一行两侧的边界点得到Left[59]和Right[59]
void First_Line_Find(void)
{
	int temp;
	
	//最底下一行从左往右搜索连续两个白点即为赛道左边界
	for(row = 0; row <= 187; row++)
	{
		if(picture[119][row] == WHITE1 && picture[119][row + 1] == WHITE1)
		{
			left[119] = row;
			temp = row;
			break;
		}
	}
	//从左边界往右搜索连续两个黑点即为赛道右边界
	for(row = temp; row <= 187; row++)
	{
		if(picture[119][row] == BLACK0 && picture[119][row + 1] == BLACK0)
		{
			right[119] = row;
			break;
		}
		//如果搜索右边界到第126列时认为赛道右边界在188列
		else if(row == 126)
		{
			right[119] = 187;
			break;
		}
	}
}

void Find_Next_Left()//由上一line的边界，寻找这一行最左侧的道路边界
{
	//情况一，已知行的道路边界的上一行点是黑色，即非道路点
	if(picture[line][left[line + 1]] == BLACK0 && picture[line][left[line + 1] - 1] == BLACK0)
	{
		//搜索终止点为上一行道路左边界的列数向右20格
		left_search = left[line + 1] + 20;
		//防止数据越界
		if(left_search > 187)
		{
			left_search = 187;
		}
		//上一行没有找到左边界或左边界为第0列
		if(left[line + 1] == 0)
		{
			left_before = left_none_start;
		}
		else
		{
			left_before = left[line + 1];
		}
		
		//搜索范围为上一行道路左边界的列数向右20格之内
		for(row = left_before; row < left_search; row++)
		{
		  	//找到黑白跳变点
			if(picture[line][row] != picture[line][row + 1])
			{
			  	//满足两黑两白滤波，即找到本行道路左边界
				if(picture[line][row + 1] == picture[line][row + 2] && picture[line][row] == picture[line][row - 1])
				{
					left[line] = row;
					break;
				}
			}
		}
		
		/*
		if(row == left_search)
		{
			if(flag_try_left < 2)//做两次判断，如果小于两次都出现这种向右找没找到的情况
			{
				left[line] = left[line + 1];
				flag_try_left++;
				if(left[line] == 0)
				{
				  
				}
			}
			else
			{
				if(left[line + 1] == 0)
				{
					left[line] = 0;
				}
				else
				{
					left[line] = 187;
				}
			}
		}
		*/
	}
	//情况二，已知道路边界的上一点是白色，即道路点
	if(picture[line][left[line + 1]] == WHITE1 && picture[line][left[line + 1] + 1] == WHITE1)//
	{
		if(left[line + 1] == left_Max)
		{
			//如果上一次的左道路边界是左边界阈值
			left[line] = left_Max;
		}
		else
		{
			//上次左道路边界未到达左边界阈值
			for(row = left[line + 1]; row > left_Max; row--)//向左侧寻找直到达到左边界阈值
			{
				if(picture[line][row] != picture[line][row - 1])//这一点和左侧点不同
				{
					if(picture[line][row - 1] == picture[line][row - 2] && picture[line][row] == picture[line][row + 1])//滤波
					{
						left[line] = row;
						break;
					}
				}
			}
			if(row == left_Max)//左侧也未找到边界
			{
				left[line] = left_Max;
				if(flag_none_left_start == 0 && left[line] == 0)//确定无边界的前一行的值
				{
					left_none_start = left[line + 1];
				}
			}
		}
	}
}

void Find_Next_Right()//输入line，寻找上一行最右侧的道路边界
{
	//情况一，已知行的道路边界的上一行点是黑色，即非道路点，要向左寻找边界
	if(picture[line][right[line+1]]!=WHITE1&&picture[line][right[line+1]+1]!=WHITE1)//
	{
		right_search=right[line+1]+20;
		if(right_search>187)
		{
			right_search=187;
		}
		if(right[line+1]==0)
		{
			right_before=right_none_start;
		}
		else
		{
			right_before=right[line+1];
		}
		for(row=right[line+1];row>=right[line+1]-20;row--)//向左找道路边界(推了10个像素点)如果没找到？
		{
			if(picture[line][row]!=picture[line][row-1])//找到两个像素点数值不同疑似边界
			{
				if(picture[line][row-1]==picture[line][row-2]&&picture[line][row]==picture[line][row+1])//滤波
				{
					right[line]=row;//向右推进找到了道路边界跳出循环
					break;
				}
			}
		}
		if(row==right[line+1]-30)//向左找了十个都没找到
		{
			if(flag_try_right<2)//做两次判断，如果两次都出现这种向右找没找到的情况
			{
				right[line]=right[line+1];
				flag_try_right++;
			}
			else
			{
				if(right[line+1]==187)
				{
					right[line]=187;
					if(right[line]==0)
					{
					  
					}
				}
				else
				{
					if(right[line+1]==0)
					{
						right[line]=0;
					}
					else
					{
						right[line]=187;
					}
				}
			}
		}
	}
	//情况二，已知道路边界的上一点是白色，即道路点，要向右寻找边界
	if(picture[line][right[line+1]]==WHITE1&&picture[line][right[line+1]-1]==WHITE1)//
	{
		if(right[line+1]==right_Max)//如果上一次的右道路边界是右边界阈值
		{
			right[line]=right_Max;
		}
		//上次右道路边界未到达右边界阈值
		else
		{
			for(row=right[line+1];row<right_Max;row++)//向右侧寻找直到达到右边界阈值
			{
				if(picture[line][row]!=picture[line][row+1])//这一点和右侧点不同
				{
					if(picture[line][row+1]==picture[line][row+2]&&picture[line][row]==picture[line][row-1])//滤波
					{
						right[line]=row;
						break;
					}
				}
			}
		}
		if(row==right_Max)//右侧也未找到边界
		{
			right[line]=right_Max;
			if(flag_none_right_start==0&&right[line]==0)//确定无边界的前一行的值
			{
				right_none_start=right[line+1];
			}
		}
	}
}

void Image_Dilate()
{
	int p_NorthWest, p_North, p_NorthEast, p_West, p_East, p_SouthWest, p_South, p_SouthEast, p_Sum;
	for(int line = 1; line < 120 - 1; line++)
	{
        for(int row = 1; row < 188 - 1; row++)
        {
			p_NorthWest = picture[line - 1][row - 1] / 255 * 128;
			p_North 	= picture[line - 1][row    ] / 255 * 64;
			p_NorthEast = picture[line - 1][row + 1] / 255 * 32;
			p_West 		= picture[line    ][row - 1] / 255 * 16;
			p_East 		= picture[line    ][row + 1] / 255 * 8;
			p_SouthWest = picture[line + 1][row - 1] / 255 * 4;
			p_South 	= picture[line + 1][row    ] / 255 * 2;
			p_SouthEast = picture[line + 1][row + 1] / 255 * 1;
			
			p_Sum = p_NorthWest + p_North + p_NorthEast + p_West + p_East + p_SouthWest + p_South + p_SouthEast;
			
			if(p_Sum == WHITE1)
			{
				picture[line][row] = WHITE1;
			}
			else
			{
			  	picture[line][row] = BLACK0;
			}
		}
	}
}

void Image_Erode()
{
	int p_NorthWest, p_North, p_NorthEast, p_West, p_East, p_SouthWest, p_South, p_SouthEast, p_Sum;
	for(int line = 1; line < 120 - 1; line++)
	{
        for(int row = 1; row < 188 - 1; row++)
        {
			p_NorthWest = picture[line - 1][row - 1] / 255 * 128;
			p_North 	= picture[line - 1][row    ] / 255 * 64;
			p_NorthEast = picture[line - 1][row + 1] / 255 * 32;
			p_West 		= picture[line    ][row - 1] / 255 * 16;
			p_East 		= picture[line    ][row + 1] / 255 * 8;
			p_SouthWest = picture[line + 1][row - 1] / 255 * 4;
			p_South 	= picture[line + 1][row    ] / 255 * 2;
			p_SouthEast = picture[line + 1][row + 1] / 255 * 1;
			
			p_Sum = p_NorthWest + p_North + p_NorthEast + p_West + p_East + p_SouthWest + p_South + p_SouthEast;
			
			if(p_Sum == 0)
			{
				picture[line][row] = BLACK0;
			}
			else
			{
			  	picture[line][row] = WHITE1;
			}
		}
	}
}

void Image_RGB()
{
	for(int line = 0; line < 120; line++)
	{
        for(int row = 0; row < 188; row++)
        {
    
            int temp = (int)((float)picture[line][row] / 256.0f * 32.0f);
            picture_rgb[line][row] = temp + (temp << 5) + (temp << 11);
        }
        picture_rgb[line][left[line]] = (0x01 << 11) + 0x07FF;
        picture_rgb[line][middle[line]] = (0x01 << 5) + 0xF81F;
        picture_rgb[line][right[line]] = 0x01 + 0xFFE0;
    }
}

void Send_Picture_Bin()
{
	//蓝牙发送图像	逐飞
	//uart_putchar(uart4, 0x00);uart_putchar(uart4, 0xff);uart_putchar(uart4, 0x01);uart_putchar(uart4, 0x01);//发送命令
    //uart_putbuff(uart4, (uint8_t *)picture, ROW*COL);  //发送图像

	//蓝牙发送图像	山外
    uart_putchar(uart4, 0x01);uart_putchar(uart4, 0xFE);
    uart_putbuff(uart4, (uint8_t *)picture, ROW*COL);  //发送图像
    uart_putchar(uart4, 0xFE);uart_putchar(uart4, 0x01);
}

void Send_Picture_RGB()
{
    uint8 temp;
	//蓝牙发送图像	山外
    uart_putchar(uart4, 0x01);uart_putchar(uart4, 0xFE);
	
	for(int i = 0; i < 120; i++)
	{
		for(int j = 0; j < 188; j++)
        {
            temp = picture_rgb[i][j] / 0xFF;
            uart_putchar(uart4, temp);
            temp = picture_rgb[i][j] & 0xFF;
            uart_putchar(uart4, temp);
        }
    }
	
    uart_putchar(uart4, 0xFE);uart_putchar(uart4, 0x01);
}
