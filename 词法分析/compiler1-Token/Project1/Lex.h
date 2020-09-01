#pragma once
#include<string>
#include<map>
#include<vector>
#include<iostream>
#include<fstream>
using namespace std;
struct CToken
{
	int m_iKind;    // ���ʵ����ͣ�Ҳ���ǵ��ʵ�ID
	std::string m_szContent;    //���ʱ�ע
	LineInfo m_LineInfo;        //�����к���Ϣ
};
struct LineInfo
{
	int m_iRow;					//���������к�
	std::string m_szFileName;   // �������ڵ�Դ�����ļ���
};
class Clex
{
private:
	int m_szLexTb[50][130];                  // ת����
	std::string m_szSource;						// Դ�����ַ���
	std::string m_szFileName;                 // Դ�����ļ���
	std::map<std::string, int> m_KeywordTbl;  // �ؼ��ʱ�
	std::string m_SzBuffer;						//���ʻ�����
	int m_iRow;									//��ǰ�к�
	int m_iNonTerminal;                         // ��ǰ���ս�̬���
	int m_iPos;									//Դ���뵱ǰ��ȡλ��
	std::vector<CToken>* m_pTokenList;          //������
	bool Process(int iTag);						//�ʷ���������
	void EmitToken(const int iKind, const std::string szContent, const int iRow); //��¼���ʺ���
	void EmitToken(const int iKind, const int iContent, const int iRow);			//��¼���ʺ���
	bool SearchKeyword(std::string szKeyWord, int& iPosition);						//��ѯ�ؼ��ֱ���
	void BufferBack();																//���ʻ��������˺���
public:
	Clex();									//Ĭ�Ϲ��캯��
	~Clex();								//Ĭ����������
	bool GenToken(std::vector<CToken>* pTokenList); //���ɵ���������
	void SetKeyWords(std::string szStr);		//���ùؼ��ֱ���
	void SetLexTbl(std::string szStr);			//����ת������
	void SetFileName(std::string szStr);		//����Դ�����ļ�������
};