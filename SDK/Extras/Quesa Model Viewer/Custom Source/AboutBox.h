//	About Box

#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include "MoofDialog.h"
class UrlPane;

class AboutBox : public MoofDialog
{
	typedef MoofDialog inherited;
  public:
	AboutBox(const short dialogID);

	virtual ~AboutBox();
	
	// methods which override MoofWindow methods:
	virtual void Activate( const Boolean becomingActive=true )
		{ if (!becomingActive) CloseSelf(); else inherited::Activate(); }

	// methods which override MoofDialog methods:
	virtual void	ClickItem( const short theItem );
	virtual void	DrawUserItem( const short item );
	virtual void	Idle();

	// custom methods
	void DoRegister();
	
  protected:
	UrlPane *mUrlPane;
	CursHandle mHandCursor;
};

#endif
