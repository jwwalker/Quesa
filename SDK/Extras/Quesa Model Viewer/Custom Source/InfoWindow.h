//	InfoWindow.h
//
//	This class represents the Info window, which gives the user details
//	about a model.
//

#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include "AdvWindow.h"
#include <QuesaGroup.h>

class InfoWindow : public AdvWindow
{
	typedef AdvWindow inherited;
	
  public:
	// constructor/destructor
	InfoWindow(TQ3GroupObject quesaData);
	~InfoWindow();
	
	// METHODS OVERRIDING MOOFWINDOW METHODS:
	virtual void Draw();

	// METHODS OVERRIDING ADVWINDOW METHODS:
	virtual void HandleError(OSErr err) {}

  private:
	TQ3GroupObject mQuesaData;
};

#endif // INFOWINDOW_H