#include "game_frame.h"
#include "editor_frame.h"
#include "image_mgr.h"
#include "card_data.h"
#include "deck_data.h"
#include "wx/wx.h"
#include "wx/app.h"
#include "wx/image.h"

#ifdef __WXMAC__
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace ygopro {

	class ygoApp : public wxApp {
	public:
		bool OnInit() {
#ifdef __WXMAC__
            ProcessSerialNumber PSN;
            GetCurrentProcess(&PSN);
            TransformProcessType(&PSN,kProcessTransformToForegroundApplication);
#endif
			if(!wxApp::OnInit())
                return false;
			wxInitAllImageHandlers();
			if(!commonCfg.LoadConfig("common.xml"))
                return true;
            stringCfg.LoadConfig("./conf/strings.xml", [](const wxString& name, const wxString& value) {
                if(name.Left(8) == wxT("setname_")) {
                    long setcode = 0;
                    value.ToLong(&setcode, 0);
                    dataMgr.RegisterSetCode((unsigned int)setcode, name.Right(name.Length() - 8));
                }
            });
			if(dataMgr.LoadDatas(commonCfg["database"]))
                return true;
            if(!imageMgr.LoadImageConfig(commonCfg["textures"]))
                return true;
            limitRegulationMgr.LoadLimitRegulation(commonCfg["limit_regulation"], stringCfg["eui_list_default"]);
            
			editorFrame = new EditorFrame(1080, 720);
			editorFrame->Center();
			editorFrame->Show();
			return true;
		}
	};

}

DECLARE_APP(ygopro::ygoApp)
IMPLEMENT_APP(ygopro::ygoApp)
