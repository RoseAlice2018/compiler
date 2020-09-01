#include"Lex.h"
bool Clex::GenToken(std::vector<CToken>* pTokenList)
{
	bool bTag = true;
	m_iPos = 0;
	m_pTokenList = pTokenList;
	//m_szSource = ;
	m_szSource.push_back(' ');
	m_iNonTerminal = 0;
	m_SzBuffer = "";
	m_iRow = 1;
	int TmpPos = 0;
	m_pTokenList->clear();
	while (m_iPos < m_SzBuffer.length() && bTag)
	{
		if (m_szSource[m_iPos] == '\n' && TmpPos != m_iPos)
		{
			m_iRow++;
			TmpPos = m_iPos;
		}
		m_SzBuffer.push_back(m_szSource.at(m_iPos));
		bTag = Process(m_szLexTb[m_iNonTerminal][m_szSource[m_iPos] < 128 ? m_szSource[m_iPos] : 128]);
		if (!bTag)
		{
			cout << m_szFileName << '(' << itoa(m_iRow,NULL,10) << ')' << "´Ê·¨·ÖÎö´íÎó£¬Çë¼ì²éµ¥´Ê";
			return false;
		}
		m_iPos++;
	}
	return bTag;
}
bool Clex::Process(int iTag)
{
	int iTmp = 0;
	if (iTag == -99)
		return false;
	if (iTag < 0)
	{
		BufferBack();
		//m_SzBuffer=
		if (iTag == -1)
		{
			//m_SzBuffer = ;
			if (SearchKeyword(m_SzBuffer, iTmp) == true)
				EmitToken(iTmp+40,NULL,m_iRow);
			else
			{
				if (m_SzBuffer.compare("True") == 0 || m_SzBuffer.compare("FALSE") == 0)
					// 
					;
				else
				{
					EmitToken(1, m_SzBuffer, m_iRow);
				}
				
			}

		}
		if (iTag >= -6 && iTag <= -2)
		{
			//
		}
		if (iTag >= -15 && iTag <= -7)
		{
			EmitToken(-iTag, NULL, m_iRow);
		}
		if (iTag >= -28 && iTag <= -16)
		{
			EmitToken(-iTag, NULL, m_iRow);
		}
		if (iTag == -42)
		{
			BufferBack();
			//m_SzBuffer
			//
		}
		m_SzBuffer = "";
		m_iNonTerminal = 0;
	}
	else
	{
		m_iNonTerminal = iTag;
	}
	return true;
}
bool Clex::SearchKeyword(string szKeyWord, int& iPosition)
{
	map<string, int>::iterator It = m_KeywordTbl.find(szKeyWord);
	if (It != m_KeywordTbl.end())
	{
		iPosition = It->second;
		return true;
	}
	return false;
}

void Clex::BufferBack()
{
	m_SzBuffer.erase(m_SzBuffer.end() - 1);
	m_iPos--;
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