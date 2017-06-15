
// ChildView.h : CChildView ��Ľӿ�
//

#pragma once
#pragma comment(lib, "winmm.lib")
#define NUM_BLOOD 3
#include <vector>
using namespace std;

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:

	CDC *cDC;
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	CRect m_client;
	
	class Item
	{
	public:
		CRect m_itemPos;
		CImage m_item;
		CRect m_explosionPos;
		CImage m_explosion;
		CRect dead_pos;
		bool is_visible;
		bool is_dead;
	};

	Item bg;
	Item addice;
	Item blood[NUM_BLOOD];

	class Role
	{
	public:
		Item player;
		vector<Item> bullet;
	};

	Role hero;
	Role enemy[6];
	int score;
	int life;
	int distance;
	int state;
	int state_counter;

	int game;

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void judge_hit();
	void judge_hurt();
	void judge_get();
	void judge_addice();
};


