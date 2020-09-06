#include"Lex.h"
bool Clex::GenToken(std::vector<CToken>* pTokenList)
{
	bool bTag = true;//用于判定单词是否正确的标志，初始设置为true
	m_iPos = 0;		//源程序文本的当前读取位置，初始设置为0
	m_pTokenList = pTokenList; 
	//m_szSource = ;	// 读取mszFilename指定的文件，将文件以字符串形式返回置入mszsource
	m_szSource.push_back(' ');//在mszsource尾部放入空格，以便在处理最后一个单词时，也能使用超前搜索识别
	m_iNonTerminal = 0;//转换表的开始状态置入，即当前状态
	m_SzBuffer = "";//单词缓存区清空
	m_iRow = 1;	//源程序当前行号为1 正在处理第一行
	int TmpPos = 0;//单词流清空
	m_pTokenList->clear();
	while (m_iPos < m_SzBuffer.length() && bTag)//未读到源程序末尾并且无出错单词，则继续识别单词
	{
		if (m_szSource[m_iPos] == '\n' && TmpPos != m_iPos)//如果当前读入字符是换行符，则将源程序累加行号加1
		{
			m_iRow++;
			TmpPos = m_iPos;
		}
		m_SzBuffer.push_back(m_szSource.at(m_iPos));//将当前读字符加入单词缓冲区
		bTag = Process(m_szLexTb[m_iNonTerminal][m_szSource[m_iPos] < 128 ? m_szSource[m_iPos] : 128]);//查询当前状态下读取当前字符时的转换状态，当前字符ASCII码大于128时，取128即可
		if (!bTag)
		{
			cout << m_szFileName << '(' << itoa(m_iRow,NULL,10) << ')' << "词法分析错误，请检查单词";
			return false;
		}
		m_iPos++;//当前读取字符后移一个字符
	}
	return bTag;
}
bool Clex::Process(int iTag)
{
	int iTmp = 0;//临时变量初始化为0
	if (iTag == -99)//下一个状态转移等于-99 表示当前状态不允许读取当前字符，即单词识别失败
		return false;
	if (iTag < 0)// 下一个转入状态小于0，表示当前状态下读取当前字符时进入终止状态，即单词识别完成
	{
		BufferBack();//由于超前搜索识别，单词缓冲区，当前读取位置回退一个字符
		//m_SzBuffer= //省去单词左右空格
		if (iTag == -1)//终止状态为-1，表示当前单词是标识符
		{
			//m_SzBuffer = ;//注意，这里将单词转成大写的原因是因为Pascal语言标识符是大小写不敏感
			if (SearchKeyword(m_SzBuffer, iTmp) == true)//判断当前单词是否为关键字
				EmitToken(iTmp+40,NULL,m_iRow);
			else
			{
				if (m_SzBuffer.compare("True") == 0 || m_SzBuffer.compare("FALSE") == 0)//判断当前单词是否为TRUE或FALSE（布尔常量）
					// 
					;
				else
				{
					EmitToken(1, m_SzBuffer, m_iRow);//将单词标识为标识符，加入单词流
				}
				
			}

		}
		if (iTag >= -6 && iTag <= -2)//判断单词是否为常量
		{
			//
		}
		if (iTag >= -15 && iTag <= -7)//判断单词是否为界符
		{
			EmitToken(-iTag, NULL, m_iRow);//将单词标识为运算符，并加入单词流
		}
		if (iTag >= -28 && iTag <= -16)//判断是否为运算符
		{
			EmitToken(-iTag, NULL, m_iRow);
		}
		if (iTag == -42)//判断终止状态是否为42 在这种状态下需要两次回退
		{
			BufferBack();//回退
			//m_SzBuffer //省去单词左右空格
			//			// 将单词登记为整型常量，并加入单词流，登记常量表
		}
		m_SzBuffer = "";//单词缓冲区清空，准备迎接下一个单词
		m_iNonTerminal = 0;//将当前状态置换为转换表的开始状态
	}
	else
	{
		m_iNonTerminal = iTag;//单词识别未完成 将当前状态置为iTag指示的下一个状态
	}
	return true;
}
bool Clex::SearchKeyword(string szKeyWord, int& iPosition)
{
	map<string, int>::iterator It = m_KeywordTbl.find(szKeyWord);//在关键字表中查找字符串
	if (It != m_KeywordTbl.end())
	{
		iPosition = It->second;//字符串是关键字 返回关键字的ID
		return true;
	}
	return false;
}

void Clex::BufferBack()
{
	m_SzBuffer.erase(m_SzBuffer.end() - 1);// 将单词缓冲区最后一个字符删除，即实现回退一个字符的功能
	m_iPos--;//当前位置回退一个字符
}

void Clex::EmitToken(int iKind, string sContent, int iRow)
{
	m_pTokenList->push_back(CToken(iKind,sContent,iRow,m_szFileName));
}
void Clex::EmitToken(const int iKind, const int iContent, const int iRow)
{
	string szTmp;
	char cBuffer[5];
	itoa(iContent, cBuffer, 10);
	szTmp = cBuffer;
	EmitToken(iKind, szTmp, iRow);
}