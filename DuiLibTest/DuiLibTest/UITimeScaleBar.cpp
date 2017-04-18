#include "stdafx.h"
#include "UITimeScaleBar.h" 

TTime Int2TTime(int nSecs){
	TTime t;
	t.byHour = nSecs / 3600;
	t.byMin = nSecs / 60 % 60;
	t.bySec = nSecs % 60;
	return t;
}

int TTime2Int(TTime t){
	return t.byHour*3600 + t.byMin*60 + t.bySec;
}

CTimeScaleBarUI::CTimeScaleBarUI(){
	m_nSideBlank = 10;
	m_dwScaleLineColor = 0xFFFF0000;
	m_dwScaleTextColor = 0xFFFF0000;
	m_dwTimePeriodColor = 0x80FFFFFF;
	m_dwTimePeriodBorderColor = 0xFFFFFF00;
	m_nMaxPeriod = 8;
	m_nPrecision = 300;

	m_nStarttime = 0;
	m_nEndtime = SECONDES_PER_DAY;

	m_bLBtnDown = false;
	m_bLBtnDrag = false;
	m_nTimeLBtnDown = 0;
	m_nTimeMouseMove = 0;
}

CTimeScaleBarUI::~CTimeScaleBarUI(){

}

LPCTSTR CTimeScaleBarUI::GetClass() const{
	return _T("TimeScaleBar");
}

LPVOID CTimeScaleBarUI::GetInterface(LPCTSTR pstrName){
	if (_tcscmp(pstrName, _T("TimeScaleBar")) == 0)
		return this;
	return __super::GetInterface(pstrName);
}

void CTimeScaleBarUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue){
	if (_tcscmp(pstrName, _T("sideblank")) == 0){
		LPTSTR pstr = NULL;
		m_nSideBlank = _tcstol(pstrValue, &pstr, 10);
	}else if (_tcscmp(pstrName, _T("scalelinecolor")) == 0){
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwScaleLineColor = _tcstoul(pstrValue, &pstr, 16);
	}else if (_tcscmp(pstrName, _T("scaletextcolor")) == 0){
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwScaleTextColor = _tcstoul(pstrValue, &pstr, 16);
	}else if (_tcscmp(pstrName, _T("periodcolor")) == 0){
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwTimePeriodColor = _tcstoul(pstrValue, &pstr, 16);
	}else if (_tcscmp(pstrName, _T("periodbordercolor")) == 0){
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		m_dwTimePeriodBorderColor = _tcstoul(pstrValue, &pstr, 16);
	}else if (_tcscmp(pstrName, _T("maxperiod")) == 0){
		LPTSTR pstr = NULL;
		m_nMaxPeriod = _tcstol(pstrValue, &pstr, 10);
	}else if (_tcscmp(pstrName, _T("precision")) == 0){
		LPTSTR pstr = NULL;
		m_nPrecision = _tcstol(pstrValue, &pstr, 10);
	}

	return __super::SetAttribute(pstrName, pstrValue);
}

void CTimeScaleBarUI::DoEvent(TEventUI& event){
	if (PtInValidRect(event.ptMouse)){
		if (event.Type == UIEVENT_MOUSEHOVER){
			TCHAR szTooltip[64];
			TIntPeriod tPeriod;
			if (GetPeriodByXPOS(event.ptMouse.x, tPeriod)){
				TTime tStart = Int2TTime(tPeriod.nStart);
				TTime tEnd = Int2TTime(tPeriod.nEnd);
				wsprintf(szTooltip, "%02d:%02d:%02d--%02d:%02d:%02d", 
					tStart.byHour, tStart.byMin, tStart.bySec, 
					tEnd.byHour, tEnd.byMin, tEnd.bySec);
			}else{
				TTime t = Int2TTime(GetTimeByXPos(event.ptMouse.x));
				wsprintf(szTooltip, "%02d:%02d:%02d", t.byHour, t.byMin, t.bySec);	
			}
			SetToolTip(szTooltip);
		}else if (event.Type == UIEVENT_BUTTONDOWN){
			if (m_vecPeriod.size() < m_nMaxPeriod && !GetPeriodByXPOS(event.ptMouse.x, m_tValidPeriodLBtnDown)){
					m_bLBtnDown = true;
					m_nTimeLBtnDown = GetTimeByXPos(event.ptMouse.x);
			}
		}
		else if (event.Type == UIEVENT_BUTTONUP){
			if (m_bLBtnDown && m_bLBtnDrag){
				int nCurTime = GetTimeByXPos(event.ptMouse.x);
				if (nCurTime != m_nTimeLBtnDown){
					int nStart = min(m_nTimeLBtnDown, nCurTime);
					int nEnd = max(m_nTimeLBtnDown, nCurTime);
					if (nStart < m_tValidPeriodLBtnDown.nStart)
						nStart = m_tValidPeriodLBtnDown.nStart;
					else if (nEnd > m_tValidPeriodLBtnDown.nEnd)
						nEnd = m_tValidPeriodLBtnDown.nEnd;
					AddPeriod(nStart,nEnd);
					Invalidate();
				}
			}
			m_bLBtnDown = false;
			m_bLBtnDrag = false;
		}
		else if (event.Type == UIEVENT_MOUSEMOVE){
			if (m_bLBtnDown){
				m_nTimeMouseMove = GetTimeByXPos(event.ptMouse.x);
				if (m_nTimeMouseMove < m_tValidPeriodLBtnDown.nStart)
					m_nTimeMouseMove = m_tValidPeriodLBtnDown.nStart;
				else if (m_nTimeMouseMove > m_tValidPeriodLBtnDown.nEnd)
					m_nTimeMouseMove = m_tValidPeriodLBtnDown.nEnd;
				m_bLBtnDrag = true;
				Invalidate();
			}
		}else if (event.Type == UIEVENT_RBUTTONDOWN){
			RemovePeriodByXPos(event.ptMouse.x);
			Invalidate();
		}else if (event.Type == UIEVENT_DBLCLICK){
			//TIntPeriod tPeriod;
			//if (GetPeriodByXPOS(event.ptMouse.x, tPeriod)){
			//	TCHAR szMsg[64];
			//	TTime tStart = Int2TTime(tPeriod.nStart);
			//	TTime tEnd = Int2TTime(tPeriod.nEnd);
			//	wsprintf(szMsg, "%02d:%02d:%02d--%02d:%02d:%02d", 
			//		tStart.byHour, tStart.byMin, tStart.bySec, 
			//		tEnd.byHour, tEnd.byMin, tEnd.bySec);
			//	MessageBox(m_pManager->GetPaintWindow(), szMsg, "设置时间段", MB_OKCANCEL);
			//}
		}else if (event.Type == UIEVENT_SCROLLWHEEL){
			int nCur = GetTimeByXPos(event.ptMouse.x);
			if (event.wParam == 0){
				// 滚轮向上放大2h
				if (m_nEndtime - m_nStarttime > 3600*2){
					//m_nStarttime = (m_nStarttime + nCur) / 2;
					//m_nEndtime = (m_nEndtime + nCur) / 2;
					//int nTest =0;
					Zoom(nCur,-3600*2);
				}else if (m_nEndtime - m_nStarttime > 3600){
					Zoom(nCur,-3600); // 最大放大到1h
				}
			}else if(event.wParam == 1){
				// 滚轮向下缩小2h
				Zoom(nCur,3600*2);
				//m_nStarttime = 2 * m_nStarttime - nCur;
				//m_nEndtime = 2 * m_nEndtime - nCur;
				if (m_nStarttime < 0)
					m_nStarttime = 0;
				if (m_nEndtime > SECONDES_PER_DAY)
					m_nEndtime = SECONDES_PER_DAY;
			}	
			Invalidate();
		}
	}

	return __super::DoEvent(event);
}

bool CTimeScaleBarUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl){
	__super::DoPaint(hDC, rcPaint, pStopControl);

	RECT rcPos = GetPos();
	int nStartHour = m_nStarttime / 3600;
	int nEndHour = (m_nEndtime + 3599) / 3600;
	// 画小时(1h)刻度
	for (int h = nStartHour; h <= nEndHour; h++){
		int nXPos = 0;
		if (GetXPosByTime(h * 3600, nXPos)){
			RECT rcLine;
			rcLine.left = rcLine.right = nXPos;
			rcLine.top = rcPos.top;
			rcLine.bottom = rcPos.top + GetHeight()/2;
			CRenderEngine::DrawLine(hDC, rcLine, 1, m_dwScaleLineColor);
			RECT rcText;
			rcText.left = rcLine.left - 8;
			rcText.right = rcLine.right + 8;
			rcText.top = rcLine.bottom;
			rcText.bottom = rcText.top + 10;
			TCHAR szText[16];
			wsprintf(szText, "%d", h);
			CRenderEngine::DrawText(hDC, m_pManager, rcText, szText, m_dwScaleTextColor, 0, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		// 画分钟刻度
		int nSplit = 6; // 10min
		if (m_nEndtime - m_nStarttime <= 2*3600 && m_nPrecision <= 60)
			nSplit = 60; // 1min
		else if (m_nEndtime - m_nStarttime <= 12*3600 && m_nPrecision <= 300)
			nSplit = 12; // 5min
		for (int m = 1; m < nSplit; m++){
			if (GetXPosByTime(h*3600 + m*3600/nSplit, nXPos)){
				RECT rcLine;
				rcLine.left = rcLine.right = nXPos;
				rcLine.top = rcPos.top;
				rcLine.bottom = rcPos.top + GetHeight()/4;
				CRenderEngine::DrawLine(hDC, rcLine, 1, m_dwScaleLineColor);
			}
		}
	}
	
	// 画已选择的时间段
	for (int i = 0; i < m_vecPeriod.size(); i++){
		RECT rc;
		int nXPos;
		GetXPosByTime(m_vecPeriod[i].nStart, nXPos);
		rc.left = nXPos;
		GetXPosByTime(m_vecPeriod[i].nEnd, nXPos);
		rc.right = nXPos;
		rc.top = rcPos.top + 2;
		rc.bottom = rcPos.bottom - 2;
		CRenderEngine::DrawColor(hDC, rc, m_dwTimePeriodColor);
		CRenderEngine::DrawRect(hDC, rc, 1, m_dwTimePeriodBorderColor);
	}

	// 画正在选择的时间段
	if (m_bLBtnDrag){
		RECT rc;
		int nXPos;
		GetXPosByTime(min(m_nTimeLBtnDown, m_nTimeMouseMove), nXPos);
		rc.left = nXPos;
		GetXPosByTime(max(m_nTimeLBtnDown, m_nTimeMouseMove), nXPos);
		rc.right = nXPos;
		rc.top = rcPos.top + 2;
		rc.bottom = rcPos.bottom - 2;
		CRenderEngine::DrawColor(hDC, rc, m_dwTimePeriodColor);
	}

	return true;
}

bool CTimeScaleBarUI::PtInValidRect(POINT pt){
	RECT rcPos = GetPos();
	if (pt.x >= rcPos.left + m_nSideBlank && pt.x <= rcPos.right - m_nSideBlank)
		return true;
	return false;
}

bool CTimeScaleBarUI::PtInPeriod(POINT pt){
	int nCurTime = GetTimeByXPos(pt.x);
	for (int i = 0; i < m_vecPeriod.size(); i++){
		if (nCurTime >= m_vecPeriod[i].nStart && nCurTime <= m_vecPeriod[i].nEnd){
			return true;
		}
	}
	return false;
}

bool CTimeScaleBarUI::GetXPosByTime(int nSecs, int& xPos){
	RECT rcPos = GetPos();
	if (nSecs < m_nStarttime){
		xPos = rcPos.left + m_nSideBlank;
		return false;
	}
	else if (nSecs > m_nEndtime){
		xPos = rcPos.right - m_nSideBlank;
		return false;
	}

	xPos = (double(nSecs - m_nStarttime)) * (GetWidth() - m_nSideBlank * 2) / (m_nEndtime - m_nStarttime) + rcPos.left + m_nSideBlank;
	return true;
}

int CTimeScaleBarUI::GetTimeByXPos(int x){
	RECT rcPos = GetPos();
	if (x < rcPos.left + m_nSideBlank || x > rcPos.right - m_nSideBlank)
		return -1; // out of range

	int nTime = (double(x - rcPos.left - m_nSideBlank)) / (GetWidth() - m_nSideBlank * 2) * (m_nEndtime - m_nStarttime) + m_nStarttime;
	return (nTime + m_nPrecision - 1 ) / m_nPrecision * m_nPrecision; // 添加精度估算
}

void CTimeScaleBarUI::AddPeriod(int nStart, int nEnd){
	if (m_vecPeriod.size() >= m_nMaxPeriod)
		return;

	TIntPeriod tPeriod;
	tPeriod.nStart = nStart;
	tPeriod.nEnd = nEnd;
	m_vecPeriod.push_back(tPeriod);
}

bool CTimeScaleBarUI::AddPeriod(const TTimePeriod& tTimePeriod){
	if (m_vecPeriod.size() >= m_nMaxPeriod)
		return false;

	TIntPeriod tPeriod;
	tPeriod.nStart = TTime2Int(tTimePeriod.tStart);;
	tPeriod.nEnd = TTime2Int(tTimePeriod.tEnd);
	m_vecPeriod.push_back(tPeriod);

	Invalidate();
}

bool CTimeScaleBarUI::GetPeriodByIndex(int index, TTimePeriod& tPeriod)
{
	if (index < 0 || index > m_vecPeriod.size())
		return false;
	tPeriod.tStart = Int2TTime(m_vecPeriod[index].nStart);
	tPeriod.tEnd = Int2TTime(m_vecPeriod[index].nEnd);
	return true;
}

void CTimeScaleBarUI::RemovePeriodByXPos(int x){
	int nCurTime = GetTimeByXPos(x);
	vector<TIntPeriod>::iterator iter = m_vecPeriod.begin();
	while (iter != m_vecPeriod.end()){
		if (nCurTime >= iter->nStart && nCurTime <= iter->nEnd){
			m_vecPeriod.erase(iter);
			return;
		}
		iter++;
	}
}

bool CTimeScaleBarUI::GetPeriodByXPOS(int x, TIntPeriod& tPeriod){
	TIntPeriod tLeft,tRight;
	tLeft.nStart = tLeft.nEnd = m_nStarttime;
	tRight.nStart = tRight.nEnd = m_nEndtime;

	int nCurTime = GetTimeByXPos(x);
	for (int i = 0; i < m_vecPeriod.size(); i++){
		if (nCurTime < m_vecPeriod[i].nStart){
			// 在时间段左边，找右边最小的时间段
			if (m_vecPeriod[i].nStart < tRight.nStart){
				tRight = m_vecPeriod[i];
			}
		}else if (nCurTime > m_vecPeriod[i].nEnd){
			// 在时间段右边，找左边最大的时间段
			if (m_vecPeriod[i].nStart > tLeft.nStart){
				tLeft = m_vecPeriod[i];
			}
		}else{
			// 在时间段中，返回该时间段
			tPeriod = m_vecPeriod[i];
			return true;
		}
	}

	// 返回当前位置的剩余时间段
	tPeriod.nStart = tLeft.nEnd;
	tPeriod.nEnd = tRight.nStart;
	return false;
}

void CTimeScaleBarUI::Zoom(int nCurTime, int nSecs){
	int A = nCurTime;
	int B = nSecs;
	double S = m_nStarttime;
	double E = m_nEndtime;
	m_nStarttime = A - (A-S)*(E-S+B)/(E-S);
	m_nEndtime   = A + (E-A)*(E-S+B)/(E-S);
}