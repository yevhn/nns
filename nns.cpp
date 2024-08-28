// nns.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

class CQDiag
{
public:
	CQDiag(unsigned int *pNN, unsigned int nCol, bool biRow);
	void node(unsigned int i, unsigned int j);
	void iline(unsigned int i, unsigned int j);
	void dline(unsigned int i, unsigned int j, unsigned int jmax);
	void jline(unsigned int i, unsigned int j);
public:
	unsigned int *m_pNN;
	unsigned int m_nCol;
private:
	bool m_biRow;
	unsigned int m_iNode;
};

CQDiag::CQDiag(unsigned int *pNN, unsigned int nCol, bool biRow)
{
	m_pNN = pNN;
	m_nCol = nCol;
	m_biRow = biRow;
	m_iNode = 0;
}

void CQDiag::node(unsigned int i, unsigned int j)
{
	m_pNN[m_biRow ? m_nCol*i+j : m_nCol*j+i] = m_iNode ++;
}

void CQDiag::iline(unsigned int i, unsigned int j)
{
	m_pNN[m_biRow ? m_nCol*i+j : m_nCol*j+i] = m_iNode ++;
	m_pNN[m_biRow ? m_nCol*i+j+1 : m_nCol*(j+1)+i] = m_iNode ++;
}

void CQDiag::dline(unsigned int i, unsigned int j, unsigned int jmax)
{
	for ( ; j <= jmax; i --, j ++ )
		m_pNN[m_biRow ? m_nCol*i+j : m_nCol*j+i] = m_iNode ++;
}

void CQDiag::jline(unsigned int i, unsigned int j)
{
	m_pNN[m_biRow ? m_nCol*i+j : m_nCol*j+i] = m_iNode ++;
	m_pNN[m_biRow ? m_nCol*(i-1)+j : m_nCol*j+i-1] = m_iNode ++;
}

int main()
{
	const unsigned int nx = 6, ny = 6;

	// expand a grid by 2 additional rows and columns surrounding each boundary
	unsigned int n = ny+4, m = nx+4;

	unsigned int *pNN = (unsigned int *)calloc(n*m, sizeof(unsigned int));
	if ( !pNN )
		return 1;

	// a node numbering scheme
	CQDiag nns(pNN, m, n <= m);
	nns.node(1, 1);
	unsigned int i = 2, ni = __min(n, m);
	for ( unsigned int imax = ni-4; i <= imax; i ++ )
	{
		nns.iline(i, 0);
		nns.dline(i, 2, i);
		nns.jline(1, i);
	}
	nns.iline(ni-3, 0);
	nns.node(ni-2, 1);
	unsigned int j = 2, nj = __max(n, m);
	for ( unsigned int jmax = ni-3; jmax <= nj-3; j ++, jmax ++ )
	{
		nns.jline(ni-1, j);
		nns.dline(ni-3, j, jmax);
		nns.jline(1, jmax);
	}
	nns.node(1, nj-2);
	nns.iline(2, nj-2);
	for ( j = nj-ni+3; j <= nj-3; j ++ )
	{
		nns.jline(ni-1, j);
		nns.dline(ni-3, j, nj-3);
		nns.iline(ni+j-nj, nj-2);
	}
	nns.node(ni-2, nj-2);

	// print a result
	int numDg = (int)log10(n*m-1)+1;
	for ( i = 0; i < n; i ++ )
	{
		for ( j = 0; j < m; j ++ )
			printf("%*d ", numDg, pNN[m*i+j]);
		printf("\n");
	}

	free(pNN);
	return 0;
}
