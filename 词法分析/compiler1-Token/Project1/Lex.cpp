#include"Lex.h"
bool Clex::GenToken(std::vector<CToken>* pTokenList)
{
	bool bTag = true;//�����ж������Ƿ���ȷ�ı�־����ʼ����Ϊtrue
	m_iPos = 0;		//Դ�����ı��ĵ�ǰ��ȡλ�ã���ʼ����Ϊ0
	m_pTokenList = pTokenList; 
	//m_szSource = ;	// ��ȡmszFilenameָ�����ļ������ļ����ַ�����ʽ��������mszsource
	m_szSource.push_back(' ');//��mszsourceβ������ո��Ա��ڴ������һ������ʱ��Ҳ��ʹ�ó�ǰ����ʶ��
	m_iNonTerminal = 0;//ת����Ŀ�ʼ״̬���룬����ǰ״̬
	m_SzBuffer = "";//���ʻ��������
	m_iRow = 1;	//Դ����ǰ�к�Ϊ1 ���ڴ����һ��
	int TmpPos = 0;//���������
	m_pTokenList->clear();
	while (m_iPos < m_SzBuffer.length() && bTag)//δ����Դ����ĩβ�����޳����ʣ������ʶ�𵥴�
	{
		if (m_szSource[m_iPos] == '\n' && TmpPos != m_iPos)//�����ǰ�����ַ��ǻ��з�����Դ�����ۼ��кż�1
		{
			m_iRow++;
			TmpPos = m_iPos;
		}
		m_SzBuffer.push_back(m_szSource.at(m_iPos));//����ǰ���ַ����뵥�ʻ�����
		bTag = Process(m_szLexTb[m_iNonTerminal][m_szSource[m_iPos] < 128 ? m_szSource[m_iPos] : 128]);//��ѯ��ǰ״̬�¶�ȡ��ǰ�ַ�ʱ��ת��״̬����ǰ�ַ�ASCII�����128ʱ��ȡ128����
		if (!bTag)
		{
			cout << m_szFileName << '(' << itoa(m_iRow,NULL,10) << ')' << "�ʷ������������鵥��";
			return false;
		}
		m_iPos++;//��ǰ��ȡ�ַ�����һ���ַ�
	}
	return bTag;
}
bool Clex::Process(int iTag)
{
	int iTmp = 0;//��ʱ������ʼ��Ϊ0
	if (iTag == -99)//��һ��״̬ת�Ƶ���-99 ��ʾ��ǰ״̬�������ȡ��ǰ�ַ���������ʶ��ʧ��
		return false;
	if (iTag < 0)// ��һ��ת��״̬С��0����ʾ��ǰ״̬�¶�ȡ��ǰ�ַ�ʱ������ֹ״̬��������ʶ�����
	{
		BufferBack();//���ڳ�ǰ����ʶ�𣬵��ʻ���������ǰ��ȡλ�û���һ���ַ�
		//m_SzBuffer= //ʡȥ�������ҿո�
		if (iTag == -1)//��ֹ״̬Ϊ-1����ʾ��ǰ�����Ǳ�ʶ��
		{
			//m_SzBuffer = ;//ע�⣬���ｫ����ת�ɴ�д��ԭ������ΪPascal���Ա�ʶ���Ǵ�Сд������
			if (SearchKeyword(m_SzBuffer, iTmp) == true)//�жϵ�ǰ�����Ƿ�Ϊ�ؼ���
				EmitToken(iTmp+40,NULL,m_iRow);
			else
			{
				if (m_SzBuffer.compare("True") == 0 || m_SzBuffer.compare("FALSE") == 0)//�жϵ�ǰ�����Ƿ�ΪTRUE��FALSE������������
					// 
					;
				else
				{
					EmitToken(1, m_SzBuffer, m_iRow);//�����ʱ�ʶΪ��ʶ�������뵥����
				}
				
			}

		}
		if (iTag >= -6 && iTag <= -2)//�жϵ����Ƿ�Ϊ����
		{
			//
		}
		if (iTag >= -15 && iTag <= -7)//�жϵ����Ƿ�Ϊ���
		{
			EmitToken(-iTag, NULL, m_iRow);//�����ʱ�ʶΪ������������뵥����
		}
		if (iTag >= -28 && iTag <= -16)//�ж��Ƿ�Ϊ�����
		{
			EmitToken(-iTag, NULL, m_iRow);
		}
		if (iTag == -42)//�ж���ֹ״̬�Ƿ�Ϊ42 ������״̬����Ҫ���λ���
		{
			BufferBack();//����
			//m_SzBuffer //ʡȥ�������ҿո�
			//			// �����ʵǼ�Ϊ���ͳ����������뵥�������Ǽǳ�����
		}
		m_SzBuffer = "";//���ʻ�������գ�׼��ӭ����һ������
		m_iNonTerminal = 0;//����ǰ״̬�û�Ϊת����Ŀ�ʼ״̬
	}
	else
	{
		m_iNonTerminal = iTag;//����ʶ��δ��� ����ǰ״̬��ΪiTagָʾ����һ��״̬
	}
	return true;
}
bool Clex::SearchKeyword(string szKeyWord, int& iPosition)
{
	map<string, int>::iterator It = m_KeywordTbl.find(szKeyWord);//�ڹؼ��ֱ��в����ַ���
	if (It != m_KeywordTbl.end())
	{
		iPosition = It->second;//�ַ����ǹؼ��� ���عؼ��ֵ�ID
		return true;
	}
	return false;
}

void Clex::BufferBack()
{
	m_SzBuffer.erase(m_SzBuffer.end() - 1);// �����ʻ��������һ���ַ�ɾ������ʵ�ֻ���һ���ַ��Ĺ���
	m_iPos--;//��ǰλ�û���һ���ַ�
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