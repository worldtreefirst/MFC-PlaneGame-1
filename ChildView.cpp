
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "My.h"
#include "ChildView.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define MUSIC

// CChildView

CChildView::CChildView()
{
	srand((unsigned)time(NULL));
#ifdef MUSIC
	mciSendString(_T("play res/bgm.wav"), 0, 0, NULL); 
#endif // MUSIC
	score = 0;
	life = NUM_LIFE;
	distance = 0;
	state = 0;
	game = 1;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void TransparentPNG(CImage *png) {
	for (int i = 0; i < png->GetWidth(); i++) {
		for (int j = 0; j < png->GetHeight(); j++) {
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(png->GetPixelAddress(i, j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}

int rand_pos(int a, int b) {
	int c = (rand() % (a - b)) + a;
	return c;
}

// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);
	
	bg.m_item.Load(_T("res/background.jpg"));
	addice.m_item.Load(_T("res/addice.png"));
	for (int i = 0; i < NUM_BLOOD; i++) {
		blood[i].m_item.Load(_T("res/blood.png"));
	}
	
	hero.player.m_item.Load(_T("res/hero.png"));
	TransparentPNG(&hero.player.m_item);

	Item item_bullet;
	int num_bullet;
	vector<Item>::iterator iter;
	int iter_no;

	for (int i = 0; i < NUM_PLANE; i++) {
		num_bullet = NUM_BULLET;
		if (i % 3 == 0) {
			enemy[i].player.m_item.Load(_T("res/ep_1.png"));
			enemy[i].player.m_explosion.Load(_T("res/explosion.png"));
		}
			
		else if (i % 3 == 1) {
			enemy[i].player.m_item.Load(_T("res/ep_2.png"));
			enemy[i].player.m_explosion.Load(_T("res/explosion.png"));
		}
		else {
			enemy[i].player.m_item.Load(_T("res/ep_3.png"));
			enemy[i].player.m_explosion.Load(_T("res/explosion.png"));
		}
		TransparentPNG(&enemy[i].player.m_item);
		while (num_bullet--)
			enemy[i].bullet.push_back(item_bullet);
		for (iter = enemy[i].bullet.begin(), iter_no = 0; iter != enemy[i].bullet.end(); iter++, iter_no++) {
			enemy[i].bullet[iter_no].m_item.Load(_T("res/e_b.png"));
			TransparentPNG(&enemy[i].bullet[iter_no].m_item);
			TransparentPNG(&enemy[i].bullet[iter_no].m_explosion);
		}
	}

	num_bullet = NUM_BULLET_MAX;
	while (num_bullet--)
		hero.bullet.push_back(item_bullet);
	
	for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
		hero.bullet[iter_no].m_item.Load(_T("res/bullet.png"));
		TransparentPNG(&hero.bullet[iter_no].m_item);
		hero.bullet[iter_no].is_visible = false;
	}

	bg.m_itemPos.left = 0;
	bg.m_itemPos.right = SCR_WIDTH;
	bg.m_itemPos.top = -SCR_HEIGHT;
	bg.m_itemPos.bottom = SCR_HEIGHT;

	addice.m_itemPos.left = rand_pos(0, 800);
	addice.m_itemPos.right = addice.m_itemPos.left + ICE_WIDTH;
	addice.m_itemPos.top = rand_pos(-600, 0);
	addice.m_itemPos.bottom = addice.m_itemPos.top + ICE_HEIGHT;
	addice.is_visible = true;

	for (int i = 0; i < NUM_BLOOD; i++) {
		blood[i].m_itemPos.left = rand_pos(0, 800);
		blood[i].m_itemPos.right = blood[i].m_itemPos.left + BLOOD_WIDTH;
		blood[i].m_itemPos.top = rand_pos(-600, 0);
		blood[i].m_itemPos.bottom = blood[i].m_itemPos.top + BLOOD_HEIGHT;
		blood[i].is_visible = true;
	}

	hero.player.m_itemPos.left = 400;
	hero.player.m_itemPos.right = 400 + HERO_WIDTH;
	hero.player.m_itemPos.top = 450;
	hero.player.m_itemPos.bottom = 450 + HERO_HEIGHT;

 
	for (int i = 0; i < NUM_PLANE; i++) {
		enemy[i].player.m_itemPos.left = rand_pos(0, 800);
		enemy[i].player.m_itemPos.right = enemy[i].player.m_itemPos.left + PLANE_WIDTH;
		enemy[i].player.m_itemPos.top = rand_pos(-600, 0);
		enemy[i].player.m_itemPos.bottom = enemy[i].player.m_itemPos.top + PLANE_HEIGHT;

		enemy[i].player.m_explosionPos.left = enemy[i].player.m_itemPos.left;
		enemy[i].player.m_explosionPos.right = enemy[i].player.m_itemPos.right;
		enemy[i].player.m_explosionPos.top = enemy[i].player.m_itemPos.top;
		enemy[i].player.m_explosionPos.bottom = enemy[i].player.m_itemPos.bottom;

		enemy[i].player.dead_pos.left = enemy[i].player.m_itemPos.left;
		enemy[i].player.dead_pos.right = enemy[i].player.m_itemPos.right;
		enemy[i].player.dead_pos.top = enemy[i].player.m_itemPos.top;
		enemy[i].player.dead_pos.bottom = enemy[i].player.m_itemPos.bottom;

		
		for (int j = 0; j < NUM_BULLET; j++) {
			enemy[i].bullet[j].m_itemPos.left = enemy[i].player.m_itemPos.left + 30;
			enemy[i].bullet[j].m_itemPos.right = enemy[i].bullet[j].m_itemPos.left + BULLET_WIDTH;
			enemy[i].bullet[j].m_itemPos.top = enemy[i].player.m_itemPos.bottom;
			enemy[i].bullet[j].m_itemPos.bottom = enemy[i].bullet[j].m_itemPos.top + BULLET_HEIGHT;
			enemy[i].bullet[j].is_visible = false;
		}
		enemy[i].player.is_visible = true;
		enemy[i].player.is_dead = false;
		
	}

	for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
		hero.bullet[iter_no].m_itemPos.left = 400 + 40;
		hero.bullet[iter_no].m_itemPos.right = 400 + 40 + BULLET_WIDTH;
		hero.bullet[iter_no].m_itemPos.top = 450 - 10;
		hero.bullet[iter_no].m_itemPos.bottom = 450 - 10 + BULLET_HEIGHT;
	}

	return TRUE;
}

void CChildView::OnPaint() 
{
	cDC = this->GetDC();
	GetClientRect(&m_client);

	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);

	bg.m_item.Draw(m_cacheDC, bg.m_itemPos);
	if (addice.is_visible)
		addice.m_item.Draw(m_cacheDC, addice.m_itemPos);
	for (int i = 0; i < NUM_BLOOD; i++) {
		if (blood[i].is_visible)
			blood[i].m_item.Draw(m_cacheDC, blood[i].m_itemPos);
	}
	hero.player.m_item.Draw(m_cacheDC, hero.player.m_itemPos);

	m_cacheDC.SetBkMode(TRANSPARENT);
	m_cacheDC.SetTextColor(RGB(0, 255, 0));

	m_cacheDC.TextOut(0, 0, _T("Score:"));
	CString str;
	str.Format(_T("%d"), score);
	m_cacheDC.TextOut(50, 0, str);

	m_cacheDC.TextOut(0, 20, _T("Blood:"));
	str.Format(_T("%d"), life);
	m_cacheDC.TextOut(50, 20, str);

	m_cacheDC.TextOut(0, 40, _T("Distance:"));
	str.Format(_T("%d"), distance);
	m_cacheDC.TextOut(70, 40, str);
	
	if (game == 0) {
		m_cacheDC.SetTextColor(RGB(255, 0, 0));
		m_cacheDC.SetTextAlign(TA_CENTER);
		m_cacheDC.TextOut(450, 250, _T("GAME OVER"));
	}

	vector<Item>::iterator iter;
	int iter_no;

	for (int i = 0; i < NUM_PLANE; i++) {
		if (enemy[i].player.is_visible) {
			enemy[i].player.m_item.Draw(m_cacheDC, enemy[i].player.m_itemPos);
			for (iter = enemy[i].bullet.begin(), iter_no = 0; iter != enemy[i].bullet.end(); iter++, iter_no++) {
				if (enemy[i].bullet[iter_no].is_visible)
					enemy[i].bullet[iter_no].m_item.Draw(m_cacheDC, enemy[i].bullet[iter_no].m_itemPos);
			}
		}
		if (enemy[i].player.is_dead) {
			if (enemy[i].player.m_itemPos.bottom < enemy[i].player.dead_pos.bottom)
			enemy[i].player.m_explosion.Draw(m_cacheDC, enemy[i].player.m_explosionPos);
		}
	}

	for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
		if (hero.bullet[iter_no].is_visible)
			hero.bullet[iter_no].m_item.Draw(m_cacheDC, hero.bullet[iter_no].m_itemPos);
	}
	
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);
	
	ValidateRect(&m_client);
	m_cacheDC.DeleteDC();
	m_cacheCBitmap.DeleteObject();
	ReleaseDC(cDC);
	// 不要为绘制消息而调用 CWnd::OnPaint()

}



void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	//nChar表示按下的键值
	if (game == 0) {
		return;
	}
	vector<Item>::iterator iter;
	int iter_no;

	switch (nChar) {
	case 'D':
	case VK_RIGHT:
		if (hero.player.m_itemPos.right != 880) {
			hero.player.m_itemPos.left += LENGTH_PACE;
			hero.player.m_itemPos.right += LENGTH_PACE;
			for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
				if (!hero.bullet[iter_no].is_visible) {
					hero.bullet[iter_no].m_itemPos.left += LENGTH_PACE;
					hero.bullet[iter_no].m_itemPos.right += LENGTH_PACE;
				}
			}
		}
		break;
	case 'A':
	case VK_LEFT:
		if (hero.player.m_itemPos.left != 0) {
			hero.player.m_itemPos.left -= LENGTH_PACE;
			hero.player.m_itemPos.right -= LENGTH_PACE;

			for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
				if (!hero.bullet[iter_no].is_visible) {
					hero.bullet[iter_no].m_itemPos.left -= LENGTH_PACE;
					hero.bullet[iter_no].m_itemPos.right -= LENGTH_PACE;
				}
			}
		}
		break;
	case 'W':
	case VK_UP:
		if (hero.player.m_itemPos.top != 0) {
			hero.player.m_itemPos.top -= LENGTH_PACE;
			hero.player.m_itemPos.bottom -= LENGTH_PACE;
			for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
				if (!hero.bullet[iter_no].is_visible) {
					hero.bullet[iter_no].m_itemPos.top -= LENGTH_PACE;
					hero.bullet[iter_no].m_itemPos.bottom -= LENGTH_PACE;
				}
			}
		}
		break;
	case 'S':
	case VK_DOWN:
		if (hero.player.m_itemPos.bottom != 550) {
			hero.player.m_itemPos.top += LENGTH_PACE;
			hero.player.m_itemPos.bottom += LENGTH_PACE;
			for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
				if (!hero.bullet[iter_no].is_visible) {
					hero.bullet[iter_no].m_itemPos.top += LENGTH_PACE;
					hero.bullet[iter_no].m_itemPos.bottom += LENGTH_PACE;
				}
			}
		}
		break;

	case VK_SPACE:
		for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
			if (!hero.bullet[iter_no].is_visible) {
				hero.bullet[iter_no].is_visible = true;
				hero.bullet[iter_no].m_itemPos.left = hero.player.m_itemPos.left + 40;
				hero.bullet[iter_no].m_itemPos.right = hero.player.m_itemPos.left + 40 + 20;
				hero.bullet[iter_no].m_itemPos.top = hero.player.m_itemPos.top - 10;
				hero.bullet[iter_no].m_itemPos.bottom = hero.player.m_itemPos.top - 10 + 20;
				break;
			}
		}
		break;
	}
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	hero.player.m_itemPos.left = point.x;
	hero.player.m_itemPos.right = hero.player.m_itemPos.left + HERO_WIDTH;
	hero.player.m_itemPos.top = point.y;
	hero.player.m_itemPos.bottom = hero.player.m_itemPos.top + HERO_HEIGHT;
	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CWnd::OnTimer(nIDEvent);
	if (life == 0) {
		game = 0;
	}

	if (bg.m_itemPos.top == 0) {
		bg.m_itemPos.bottom = SCR_HEIGHT;
		bg.m_itemPos.top = -SCR_HEIGHT;
	}

	if (state) {
		state_counter--;
		if (!state_counter)
			state = 0;
	}
	
	if (!state) {
		if (game)
			distance += 1;
		bg.m_itemPos.bottom += 1;
		bg.m_itemPos.top += 1;
		addice.m_itemPos.bottom += 3;
		addice.m_itemPos.top += 3;
		for (int i = 0; i < NUM_BLOOD; i++) {
			blood[i].m_itemPos.bottom += 2;
			blood[i].m_itemPos.top += 2;
		}

		for (int i = 0; i < NUM_PLANE; i++) {
			enemy[i].player.m_itemPos.top += 2;
			enemy[i].player.m_itemPos.bottom += 2;
			enemy[i].player.m_explosionPos.top += 2;
			enemy[i].player.m_explosionPos.bottom += 2;

			for (int j = 0; j < NUM_BULLET; j++) {
				enemy[i].bullet[j].m_itemPos.left = enemy[i].player.m_itemPos.left + 30;
				enemy[i].bullet[j].m_itemPos.right = enemy[i].bullet[j].m_itemPos.left + BULLET_WIDTH;
				enemy[i].bullet[j].m_itemPos.top += 2;
				enemy[i].bullet[j].m_itemPos.bottom += 2;
			}

			if (enemy[i].player.m_itemPos.top > 600) {
				enemy[i].player.m_itemPos.left = rand_pos(0, 800);
				enemy[i].player.m_explosionPos.left = enemy[i].player.m_itemPos.left;
				enemy[i].player.m_itemPos.right = enemy[i].player.m_itemPos.left + PLANE_WIDTH;
				enemy[i].player.m_explosionPos.right = enemy[i].player.m_itemPos.right;
				enemy[i].player.m_itemPos.top = rand_pos(-100, 0);
				enemy[i].player.m_explosionPos.top = enemy[i].player.m_itemPos.top;
				enemy[i].player.m_itemPos.bottom = enemy[i].player.m_itemPos.top + PLANE_HEIGHT;
				enemy[i].player.m_explosionPos.bottom = enemy[i].player.m_itemPos.bottom;

				for (int j = 0; j < NUM_BULLET; j++) {
					enemy[i].bullet[j].m_itemPos.left = enemy[i].player.m_itemPos.left + 30;
					enemy[i].bullet[j].m_itemPos.right = enemy[i].bullet[j].m_itemPos.left + BULLET_WIDTH;
					enemy[i].bullet[j].m_itemPos.top = enemy[i].player.m_itemPos.bottom;
					enemy[i].bullet[j].m_itemPos.bottom = enemy[i].bullet[j].m_itemPos.top + BULLET_HEIGHT;
					enemy[i].bullet[j].is_visible = true;
				}
				enemy[i].player.is_visible = true;
				enemy[i].player.is_dead = false;
			}
		}

		for (int i = 0; i < NUM_PLANE; i++) {
			if (enemy[i].player.is_visible) {
				for (int j = 0; j < NUM_BULLET; j++) {
					enemy[i].bullet[j].is_visible = true;
				}
				SetTimer(TIMER_SHOOT, 10, NULL);
			}
		}
	}	

	judge_hit();
	judge_hurt();
	judge_get();
	judge_addice();

	switch (nIDEvent)
	{
	case TIMER_SHOOT:
		for (int i = 0; i < NUM_PLANE; i++) {
			for (int j = 0; j < NUM_BULLET; j++) {
				enemy[i].bullet[j].m_itemPos.top += 3 + j;
				enemy[i].bullet[j].m_itemPos.bottom += 3 + j;
			}
		}
	case TIMER_PAINT:
		OnPaint();
		break;
	}
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(TIMER_PAINT, 10, NULL);
	
	return 0;
}

void CChildView::judge_hit()
{
	if (!game)
		return;
	vector<Item>::iterator iter;
	int iter_no;
	for (iter = hero.bullet.begin(), iter_no = 0; iter != hero.bullet.end(); iter++, iter_no++) {
		if (hero.bullet[iter_no].is_visible) {
			for (int i = 0; i < NUM_PLANE; i++) {
				if (enemy[i].player.is_visible)
					if (hero.bullet[iter_no].m_itemPos.left > enemy[i].player.m_itemPos.left
						&& hero.bullet[iter_no].m_itemPos.right < enemy[i].player.m_itemPos.right
						&& hero.bullet[iter_no].m_itemPos.top <= enemy[i].player.m_itemPos.bottom) {
						enemy[i].player.is_visible = false;
						enemy[i].player.is_dead = true;
						enemy[i].player.dead_pos = enemy[i].player.m_itemPos;
						enemy[i].player.dead_pos.bottom = enemy[i].player.m_itemPos.bottom + 100;
						enemy[i].player.dead_pos.top = enemy[i].player.m_itemPos.top + 100;
						hero.bullet[iter_no].is_visible = false;
						mciSendString(_T("play res/hit.wav"), 0, 0, NULL);
						score += 5;
					}
			}
			hero.bullet[iter_no].m_itemPos.bottom -= 8;
			hero.bullet[iter_no].m_itemPos.top -= 8;
		}
		if (hero.bullet[iter_no].m_itemPos.top < 0) {
			hero.bullet[iter_no].is_visible = false;
			hero.bullet[iter_no].m_itemPos.top = 300;
			hero.bullet[iter_no].m_itemPos.bottom = 320;
		}
	}
}

void CChildView::judge_hurt()
{
	if (!game)
		return;
	for (int i = 0; i < NUM_PLANE; i++) {
		if (enemy[i].player.is_visible) {
			if ((enemy[i].player.m_itemPos.top <= hero.player.m_itemPos.bottom
				&& enemy[i].player.m_itemPos.bottom >= hero.player.m_itemPos.bottom
				|| enemy[i].player.m_itemPos.bottom >= hero.player.m_itemPos.top
				&& enemy[i].player.m_itemPos.top <= hero.player.m_itemPos.top)
				&& (enemy[i].player.m_itemPos.left <= hero.player.m_itemPos.right
					&& enemy[i].player.m_itemPos.right >= hero.player.m_itemPos.right
					|| enemy[i].player.m_itemPos.right >= hero.player.m_itemPos.left
					&& enemy[i].player.m_itemPos.left <= hero.player.m_itemPos.left)) {
				enemy[i].player.is_visible = false;
				if (life - 5 < 0)
					life = 0;
				else {
					life -= 5;
				}
			}
				
			else {
				for (int j = 0; j < NUM_BULLET; j++) {
					if (enemy[i].bullet[j].m_itemPos.left >= hero.player.m_itemPos.left
						&& enemy[i].bullet[j].m_itemPos.right <= hero.player.m_itemPos.right
						&& enemy[i].bullet[j].m_itemPos.top <= hero.player.m_itemPos.bottom
						&& enemy[i].bullet[j].m_itemPos.top >= hero.player.m_itemPos.top) {
						if (life > 0)
							life--;
						enemy[i].bullet[j].is_visible = false;
					}
				}
			}
		}
	}
}

void CChildView::judge_get()
{
	if (!game)
		return;
	for (int i = 0; i < NUM_BLOOD; i++) {
		if (blood[i].is_visible) {
			if (blood[i].m_itemPos.bottom >= hero.player.m_itemPos.top
				&& blood[i].m_itemPos.top <= hero.player.m_itemPos.bottom
				&& blood[i].m_itemPos.left >= hero.player.m_itemPos.left
				&& blood[i].m_itemPos.right <= hero.player.m_itemPos.right) {
				blood[i].is_visible = false;
				life += 50;
			}
		}
		if (blood[i].m_itemPos.top > 600) {
			blood[i].is_visible = true;
			blood[i].m_itemPos.left = rand_pos(0, 800);
			blood[i].m_itemPos.right = blood[i].m_itemPos.left + BLOOD_WIDTH;
			blood[i].m_itemPos.top = rand_pos(-600, 0);
			blood[i].m_itemPos.bottom = blood[i].m_itemPos.top + BLOOD_HEIGHT;
		}
	}
}

void CChildView::judge_addice()
{
	if (!game)
		return;
	if (addice.is_visible) {
		if (addice.m_itemPos.bottom >= hero.player.m_itemPos.top
			&& addice.m_itemPos.top <= hero.player.m_itemPos.bottom
			&& addice.m_itemPos.left >= hero.player.m_itemPos.left
			&& addice.m_itemPos.right <= hero.player.m_itemPos.right) {
			addice.is_visible = false;
			state = 1;
			state_counter = 500;
		}
	}
	if (addice.m_itemPos.top > 600) {
		addice.is_visible = true;
		addice.m_itemPos.left = rand_pos(0, 800);
		addice.m_itemPos.right = addice.m_itemPos.left + ICE_WIDTH;
		addice.m_itemPos.top = rand_pos(-600, 0);
		addice.m_itemPos.bottom = addice.m_itemPos.top + ICE_HEIGHT;
	}
}