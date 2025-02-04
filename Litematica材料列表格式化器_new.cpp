#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>


bool FlushInput(void)
{
	int c;
	while ((c = getchar()) != '\n')
	{
		if (c == EOF)
		{
			return false;
		}
	}
	return true;
}

typedef struct level
{
	union
	{
		struct
		{
			long lLargeChestShulkerBox;
			long lSmallChestShulkerBox;
			long lShulkerBox;
			long lSetItem;
			long lItem;
		};
		long lData[5];
	};
	bool bNegSign;
}LEVEL, *PLEVEL;

const static char *cLevel[sizeof(LEVEL::lData) / sizeof(long)] =
{
	"大箱盒","箱盒","盒","组","个",
};

enum ecell
{
	ChestCellCount = 27,
	ShulkerBoxCellCount = ChestCellCount,
	LargeChestCellCount = ChestCellCount * 2,
};

enum eitem
{
	SetItemCount = 64,
	ShulkerBoxItemCount = ShulkerBoxCellCount * SetItemCount,
	ChestShulkerBoxItemCount = ChestCellCount * ShulkerBoxCellCount * SetItemCount,
	LargeChestShulkerBoxItemCount = LargeChestCellCount * ShulkerBoxCellCount * SetItemCount,
};

LEVEL CalculateLevels(long long number)
{
	LEVEL level = { 0 };
	if (number < 0)
	{
		level.bNegSign = true;//默认初始化NegSign为false，需要手动设置
		number = -number;
	}

	//挨个计算
	if (number >= LargeChestShulkerBoxItemCount)
	{
		level.lLargeChestShulkerBox = (number / LargeChestShulkerBoxItemCount);
	}
	if (number >= ChestShulkerBoxItemCount)
	{
		level.lSmallChestShulkerBox = (number / ChestShulkerBoxItemCount) % (LargeChestCellCount / ChestCellCount);
	}
	if (number >= ShulkerBoxItemCount)
	{
		level.lShulkerBox = (number / ShulkerBoxItemCount) % ShulkerBoxCellCount;
	}
	if (number >= SetItemCount)
	{
		level.lSetItem = (number / SetItemCount) % ShulkerBoxCellCount;
	}
	//if (number > 0)//无需判断
	{
		level.lItem = number % SetItemCount;
	}

	return level;
}


std::string Level2String(const LEVEL &level)
{
	std::string strRet;
	strRet.reserve(64);//预分配

	if (level.bNegSign)
	{
		strRet += "| 多余 ";//如果多出来则开头输出多余
	}
	else
	{
		strRet += "| ";//否则正常输出
	}

	bool bOut = false;
	for (int i = 0; i < sizeof(LEVEL::lData) / sizeof(long); ++i)
	{
		if (level.lData[i] != 0)
		{
			if (bOut)
			{
				strRet += " + ";
			}
			strRet += std::to_string(level.lData[i]);
			strRet += cLevel[i];
			bOut = true;
		}
	}

	if (!bOut)
	{
		strRet += '0';//输出0个
		strRet += cLevel[sizeof(LEVEL::lData) / sizeof(long) - 1];
	}

	if (strRet.size() - 2 <= 8)
	{
		strRet += "\t\t";
	}
	else if (strRet.size() - 2 <= 17)
	{
		strRet += '\t';
	}
	else
	{
		strRet += ' ';
	}

	return strRet;
}

int Convert(const char *cFileName)
{
	static bool bOverwrite = false;
	//先打开输入文件，然后加上_out后缀作为输出文件
	std::fstream fIn{ cFileName,std::ios::in };//只读
	if (!fIn.is_open())
	{
		printf("Unable to open input file [%s]\n", std::strerror(errno));
		return -1;
	}

	//准备输出文件名
	std::string strOutFileName{ cFileName };
	size_t szSuffixNamePos = strOutFileName.find_last_of('.');
	if (szSuffixNamePos == std::string::npos)//处理无后缀名情况
	{
		//直接拼接
		strOutFileName += "_out";
	}
	else
	{
		//在后缀名前插入
		strOutFileName.insert(szSuffixNamePos, "_out");
	}

	//打开输出文件之前确认文件是否存在，存在询问用户
	if (!bOverwrite && std::filesystem::exists(strOutFileName))
	{
		printf("The file [%s] already exists. Overwrite it? (Yes/No/All)", strOutFileName.c_str());
		while (true)
		{
			int c = getchar();
			if (c == 'y' || c == 'Y')
			{
				break;
			}
			else if (c == 'n' || c == 'N')
			{
				return -1;
			}
			else if (c == 'a' || c == 'A')
			{
				bOverwrite = true;
				break;
			}
			else
			{
				continue;
			}
		}
		//清理缓冲区
		if (!FlushInput())
		{
			return -1;
		}
	}

	//打开输出文件
	std::fstream fOut{ strOutFileName , std::ios::out | std::ios::trunc };//清空写入
	if (!fOut.is_open())
	{
		printf("Unable to open input file [%s]\n", std::strerror(errno));
		return -1;
	}

	//读取前5行，输出到文件
	for (int i = 0; i < 5; ++i)
	{
		std::string strLineTemp;
		std::getline(fIn, strLineTemp, '\n');
		if (i != 3)//正常输出
		{
			fOut << strLineTemp << '\n';
		}
		else//i为3是第四行，直接读取丢弃然后输出固定内容
		{
			fOut << "| 物品名\t\t| 总物品个数\t| 缺少的物品个数\t|\n";
		}
	}

	//接下来的每一行：|分隔符后跟的分别为：总物品数，缺少的物品数，已有的物品数
	//将最后两项合并为缺少的物品数，如果缺少的为负数则代表已有的物品多出的部分
	while (fIn.get() == '|')
	{
		//读取物品名
		std::string strItemName;
		fIn >> strItemName;//读入一个物品名（跳过空格因为mc物品名不存在空格）
		//依次读取3个数据
		long long llData[3];
		for (int i = 0; i < 3; ++i)
		{
			fIn.ignore(std::numeric_limits<std::streamsize>::max(), '|');//跳到下一分隔符
			fIn >> llData[i];
		}
		fIn.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//跳到下一行

		//计算物品数
		long long llTotal = llData[0];
		long long llMissing = llData[1] - llData[2];//Missing - Available 做为Missing

		//计算物品分组
		LEVEL lvTotal = CalculateLevels(llTotal);
		LEVEL lvMissing = CalculateLevels(llMissing);

		//输出物品信息
		fOut << "| " << strItemName;
		//对齐
		if (strItemName.size() <= 8)
		{
			fOut << "\t\t";
		}
		else if (strItemName.size() <= 17)
		{
			fOut << '\t';
		}
		else
		{
			fOut << ' ';
		}

		//输出格式化物品数目
		fOut << Level2String(lvTotal);
		fOut << Level2String(lvMissing);

		//封尾换行
		fOut << "|\n";
	}

	//到结尾了
	fIn.unget();
	std::string strLineTemp;
	std::getline(fIn, strLineTemp, '\n');
	fOut << strLineTemp << '\n';
	//剩下的不要了，直接退出程序

	//关闭文件结束
	fOut.close();
	fIn.close();

	printf("Done! [%s]\n", strOutFileName.c_str());
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printf("Need least one argument(can be multiple): material_list filename!\n");
		return -1;
	}

	for (int i = 1; i < argc; ++i)//对所有入参进行操作
	{
		(void)Convert(argv[i]);
	}

	return 0;
}