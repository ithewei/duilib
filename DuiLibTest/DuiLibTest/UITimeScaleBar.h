#ifndef __UI_TIME_SCALE_BAR_H__
#define __UI_TIME_SCALE_BAR_H__

#define SECONDES_PER_DAY	60*60*24

typedef struct tagTime
{
	byte	byHour;
	byte	byMin;
	byte	bySec;
	//byte    wMSec; // 微秒
}TTime;

typedef struct tagTimePeriod
{
	TTime tStart;
	TTime tEnd;
}TTimePeriod;

struct TIntPeriod
{
	LONG nStart;
	LONG nEnd;
};

TTime Int2TTime(int nSecs);
int TTime2Int(TTime t);

class CTimeScaleBarUI : public CControlUI
{
public:
	CTimeScaleBarUI();
	~CTimeScaleBarUI();

public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);
	bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

public:
	// 对外数据接口
	void ResetPeriod() {m_vecPeriod.clear();Invalidate();}
	bool AddPeriod(const TTimePeriod& tTimePeriod);
	int	 GetPeriodCnt() {return m_vecPeriod.size();}
	bool GetPeriodByIndex(int index, TTimePeriod& tPeriod);

protected:
	bool PtInValidRect(POINT pt);
	bool PtInPeriod(POINT pt);
	bool GetXPosByTime(int nSecs, int& xPos);
	int GetTimeByXPos(int x);
	void AddPeriod(int nStart, int nEnd);
	bool GetPeriodByXPOS(int x, TIntPeriod& tPeriod);
	void RemovePeriodByXPos(int x);

	// 缩放计算,nSecs为负放大，为正缩小
	void Zoom(int nCurTime, int nSecs);

protected:
	// 增加属性
	int m_nSideBlank; // 两边空白距离
	DWORD m_dwScaleLineColor; // 刻度线颜色
	DWORD m_dwScaleTextColor; // 刻度线字体颜色
	DWORD m_dwTimePeriodColor; // 时间段颜色
	DWORD m_dwTimePeriodBorderColor; // 时间段边框颜色
	int m_nMaxPeriod; // 最多可选择的时间段数
	int m_nPrecision; // 精度

	int m_nStarttime; // 起点表示的开始时间值(单位：s)
	int m_nEndtime; // 末端表示的结束时间值(单位：s)
	vector<TIntPeriod> m_vecPeriod; // 记录时间段信息

	bool m_bLBtnDown;
	int m_nTimeLBtnDown;
	TIntPeriod m_tValidPeriodLBtnDown;
	bool m_bLBtnDrag;
	int m_nTimeMouseMove;
};

#endif // __UI_TIME_SCALE_BAR_H__