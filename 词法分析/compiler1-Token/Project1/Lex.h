#pragma once
#include<string>
#include<map>
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;
struct CToken
{
	int m_iKind;    // 单词的类型，也就是单词的ID
	std::string m_szContent;    //单词备注
	LineInfo m_LineInfo;        //单词行号信息
};
struct LineInfo
{
	int m_iRow;					//单词所在行号
	std::string m_szFileName;   // 单词所在的源程序文件名
};
class Clex
{
private:
	int m_szLexTb[50][130];                  // 转换表
	std::string m_szSource;						// 源代码字符串
	std::string m_szFileName;                 // 源程序文件名
	std::map<std::string, int> m_KeywordTbl;  // 关键词表
	std::string m_SzBuffer;						//单词缓冲区
	int m_iRow;									//当前行号
	int m_iNonTerminal;                         // 当前非终结态编号
	int m_iPos;									//源代码当前读取位置
	std::vector<CToken>* m_pTokenList;          //单词流
	bool Process(int iTag);						//词法分析处理
	void EmitToken(const int iKind, const std::string szContent, const int iRow); //记录单词函数
	void EmitToken(const int iKind, const int iContent, const int iRow);			//记录单词函数
	bool SearchKeyword(std::string szKeyWord, int& iPosition);						//查询关键字表函数
	void BufferBack();																//单词缓冲区回退函数
public:
	Clex();									//默认构造函数
	~Clex();								//默认析构函数
	bool GenToken(std::vector<CToken>* pTokenList); //生成单词流函数
	void SetKeyWords(std::string szStr);		//设置关键字表函数
	void SetLexTbl(std::string szStr);			//设置转换表函数
	void SetFileName(std::string szStr);		//设置源程序文件名函数
};