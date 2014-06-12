/*===========================================================================*\
 | 
 |  FILE:	Texmap.cpp
 |			Skeleton project and code for a Texture Map
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Harry Denholm
 |			Developer Consulting Group
 |			Copyright(c) Discreet 1999
 |
 |  HIST:	Started 11-3-99
 | 
\*===========================================================================*/

#include "Texmap.h"
#include "debug.h"
/*===========================================================================*\
 |	Class Descriptor
\*===========================================================================*/

class StressTexmapClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic()					{ return TRUE; }
	void *			Create( BOOL loading )		{ return new StressTexmap; }
	const TCHAR *	ClassName()					{ return GetString(IDS_CLASSNAME); }
	SClass_ID		SuperClassID()				{ return TEXMAP_CLASS_ID; }
	Class_ID 		ClassID()					{ return STRESS_TEX_CLASSID; }
	const TCHAR* 	Category()					{ return _T("");  }

	// Hardwired name, used by MAX Script as unique identifier
	const TCHAR*	InternalName()				{ return _T("StressTexmap"); }
	HINSTANCE		HInstance()					{ return hInstance; }
};

static StressTexmapClassDesc SkelTexmapCD;
ClassDesc* GetStressTexmapDesc() {return &SkelTexmapCD;}

/*===========================================================================*\
 |	Basic implimentation of a dialog handler
\*===========================================================================*/


static HIMAGELIST hAboutImage = NULL;
static HIMAGELIST hHelpImage = NULL;

static void LoadImages()
{
	if (!hAboutImage) 
	{
		HBITMAP hBitmap, hMask;
		hAboutImage = ImageList_Create(16, 16, ILC_COLOR8|ILC_MASK, 3, 0);
		hBitmap     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ABOUT));
		hMask       = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_ABOUT_MASK));
		ImageList_Add(hAboutImage,hBitmap,hMask);
		DeleteObject(hBitmap);
		DeleteObject(hMask);
	}
	if (!hHelpImage) 
	{
		HBITMAP hBitmap, hMask;
		hHelpImage  = ImageList_Create(16, 16, ILC_COLOR8|ILC_MASK, 3, 0);
		hBitmap     = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_HELP));
		hMask       = LoadBitmap(hInstance,MAKEINTRESOURCE(IDB_HELP_MASK));
		ImageList_Add(hHelpImage,hBitmap,hMask);
		DeleteObject(hBitmap);
		DeleteObject(hMask);
	}
}	

#if ( MAX_RELEASE >= 9000 )
static INT_PTR CALLBACK AboutDlgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
#else
static BOOL CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
#endif
{
	switch (msg)
	{
		case WM_INITDIALOG:
			CenterWindow(hWnd,GetParent(hWnd));
		break;

		case WM_CLOSE:
			EndDialog(hWnd,1);
		break;

		default:
			return FALSE;
	}
	return TRUE;
}

INT_PTR SkelTexDlgProc::DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int id = LOWORD(wParam);
	switch (msg) 
	{
		case WM_INITDIALOG:
			InitAuthorized(hWnd);

			LoadImages();
			ICustButton *iTmp;

			iTmp = GetICustButton(GetDlgItem(hWnd,IDC_STRESS_ABOUT));
			iTmp->SetImage(hAboutImage, 0, 0, 0, 0, 16, 16);
			iTmp->SetTooltip(TRUE,_T("About UVW Frame"));

			iTmp = GetICustButton(GetDlgItem(hWnd,IDC_STRESS_HELP));
			iTmp->SetImage(hHelpImage, 0, 0, 0, 0, 16, 16);
			iTmp->SetTooltip(TRUE,_T("About UVW Frame"));

			ReleaseICustButton(iTmp);

		break;

		case WM_DESTROY:
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDC_BANNER_CRACKART:
					ShellExecute(NULL, "open", "http://www.crackart.org", NULL, NULL, SW_SHOWNORMAL);
				break;

				case IDC_STRESS_HELP:
					ShellExecute(NULL, "open", "http://www.mankua.com/stress.php", NULL, NULL, SW_SHOWNORMAL);
				break;

				case IDC_STRESS_ABOUT:
					DialogBoxParam(	hInstance, MAKEINTRESOURCE(IDD_ABOUT), GetCOREInterface()->GetMAXHWnd(), AboutDlgProc, 0);
				break;
			}
			break;
	}
	return FALSE;
}

void SkelTexDlgProc::ShowStressControls(HWND hPanel, BOOL show) {
	int status;
	if ( show )		status = SW_SHOW;
	else			status = SW_HIDE;

	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_REFTEXT),status);
	ShowWindow(GetDlgItem(hPanel,IDC_MAP_CH),status);
	ShowWindow(GetDlgItem(hPanel,IDC_MAP_CHSPIN),status);
	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_TYPE),status);
	ShowWindow(GetDlgItem(hPanel,IDC_SHOW_STRAIN),status);
	ShowWindow(GetDlgItem(hPanel,IDC_SHOW_COMP),status);
	ShowWindow(GetDlgItem(hPanel,IDC_SHOW_SUMM),status);

	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_MAX),status);
	ShowWindow(GetDlgItem(hPanel,IDC_MXSTRESS_EDIT),status);
	ShowWindow(GetDlgItem(hPanel,IDC_MXSTRESS_SPIN),status);
	ShowWindow(GetDlgItem(hPanel,IDC_STRESS_ALLEDGES),status);

	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_TENSION),status);
	ShowWindow(GetDlgItem(hPanel,IDC_TLS_STRAINCOL),status);
	ShowWindow(GetDlgItem(hPanel,IDC_STR_MAP),status);
	ShowWindow(GetDlgItem(hPanel,IDC_STR_MAPON),status);

	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_RELAX),status);
	ShowWindow(GetDlgItem(hPanel,IDC_TLS_RELAXCOL),status);
	ShowWindow(GetDlgItem(hPanel,IDC_RLX_MAP),status);
	ShowWindow(GetDlgItem(hPanel,IDC_RLX_MAPON),status);

	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_COMP),status);
	ShowWindow(GetDlgItem(hPanel,IDC_TLS_COMPCOL),status);
	ShowWindow(GetDlgItem(hPanel,IDC_CMP_MAP),status);
	ShowWindow(GetDlgItem(hPanel,IDC_CMP_MAPON),status);

	ShowWindow(GetDlgItem(hPanel,IDC_STATIC_MIX),status);
	ShowWindow(GetDlgItem(hPanel,IDC_MIX_EDIT),status);
	ShowWindow(GetDlgItem(hPanel,IDC_MIX_SPIN),status);
	}

void SkelTexDlgProc::InitAuthorized(HWND hWnd)
{
	ShowStressControls(hWnd,TRUE);
}

/*===========================================================================*\
 |	Paramblock2 Descriptor
\*===========================================================================*/

static ParamBlockDesc2 stex_param_blk ( tex_params, _T("StressTexmap parameters"),  0, &SkelTexmapCD, P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_STRESS,			IDS_PARAMETERS, 0, 0, NULL, 
	// params
	tex_str_map,		_T("strain map"),			TYPE_TEXMAP,	P_OWNERS_REF,	IDS_STR_MAP,
		p_refno,		0,
		p_subtexno,		0,		
		p_ui,			TYPE_TEXMAPBUTTON, IDC_STR_MAP,
		end,

	tex_str_mapon,		_T("strain map on/off"),	TYPE_BOOL,		0,				IDS_STR_MAPON,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_STR_MAPON,
		end,

	tex_rlx_map,		_T("relax map"),			TYPE_TEXMAP,	P_OWNERS_REF,	IDS_RLX_MAP,
		p_refno,		1,
		p_subtexno,		1,
		p_ui,			TYPE_TEXMAPBUTTON,	IDC_RLX_MAP,

		end,
	tex_rlx_mapon,		_T("relax map on/off"),		TYPE_BOOL,		0,				IDS_RLX_MAPON,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_RLX_MAPON,
		end,

	tex_cmp_map,		_T("comp map"),				TYPE_TEXMAP,	P_OWNERS_REF,	IDS_CMP_MAP,
		p_refno,		2,
		p_subtexno,		2,
		p_ui,			TYPE_TEXMAPBUTTON,	IDC_CMP_MAP,

		end,

	tex_cmp_mapon,		_T("comp map on/off"),		TYPE_BOOL,		0,				IDS_CMP_MAPON,
		p_default,		TRUE,
		p_ui,			TYPE_SINGLECHEKBOX, IDC_CMP_MAPON,
		end,

	tex_canal,			_T("tex_canal"),			TYPE_INT,		P_ANIMATABLE,	IDS_MIXPARAM,
		p_default,		1,
		p_range,		0, 100,
		p_ui,			TYPE_SPINNER, EDITTYPE_INT, IDC_MAP_CH,		IDC_MAP_CHSPIN,	1.0f,
		end,

	tex_mxstress_param,	_T("tex_mxstress_param"),	TYPE_FLOAT,		P_ANIMATABLE,	IDS_MXSTRESS,
		p_default,		100.0f,
		p_range,		0.0f, 999999.99f,
		p_ui,			TYPE_SPINNER, EDITTYPE_FLOAT, IDC_MXSTRESS_EDIT, IDC_MXSTRESS_SPIN, 0.01f,
		end,

	strain_color,	 _T("strain_color"),	TYPE_RGBA,				P_ANIMATABLE,	IDS_STRAINCOL,	
		p_default,		Color(1.0,0.0,0.0), 
		p_ui,			TYPE_COLORSWATCH, IDC_TLS_STRAINCOL, 
		end,

	relax_color,	 _T("relax_color"),		TYPE_RGBA,				P_ANIMATABLE,	IDS_RELAXCOL,	
		p_default,		Color(0.0,1.0,0.0), 
		p_ui,			TYPE_COLORSWATCH, IDC_TLS_RELAXCOL, 
		end,

	comp_color,		_T("com_color"),		TYPE_RGBA,				P_ANIMATABLE,	IDS_COMPCOL,	
		p_default,		Color(0.0,0.0,1.0), 
		p_ui,			TYPE_COLORSWATCH, IDC_TLS_COMPCOL, 
		end,

	tex_mix_param,	_T("tex_mix_param"),	TYPE_FLOAT,		P_ANIMATABLE,	IDS_MIXPARAM,
		p_default,		2.0f,
		p_range,		0.0f, 999999.99f,
		p_ui,			TYPE_SPINNER, EDITTYPE_FLOAT, IDC_MIX_EDIT, IDC_MIX_SPIN, 0.01f,
		end,

	tex_show,		_T("tex_show"), TYPE_INT,				0,				IDS_SHOW,
		p_default,		0,
		p_range,		0,	2,
		p_ui,			TYPE_RADIO, 3, IDC_SHOW_STRAIN, IDC_SHOW_COMP, IDC_SHOW_SUMM,
		end,

	end
	);



/*===========================================================================*\
 |	Constructor and Reset systems
 |  Ask the ClassDesc2 to make the AUTO_CONSTRUCT paramblocks and wire them in
\*===========================================================================*/

void StressTexmap::Reset() 
{
	ivalid.SetEmpty();
	for (int i=0; i<NSUBTEX; i++) {
		DeleteReference(i);
		mapOn[i] = 1;
		}

	SkelTexmapCD.MakeAutoParamBlocks(this);	// make and intialize paramblock2
}

StressTexmap::StressTexmap() 
{
	for (int i=0; i<NSUBTEX; i++) subTex[i] = NULL;
	pblock = NULL;
	Reset();
	surfs.SetCount(0);
	InitializeCriticalSection(&csect);
}

StressTexmap::~StressTexmap() {
	DeleteCriticalSection(&csect);
	}

ParamDlg* StressTexmap::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
	IAutoMParamDlg* masterDlg = SkelTexmapCD.CreateParamDlgs(hwMtlEdit, imp, this);
	stex_param_blk.SetUserDlgProc(new SkelTexDlgProc(this));

	return masterDlg;
}



/*===========================================================================*\
 |	Subanim & References support
\*===========================================================================*/

RefTargetHandle StressTexmap::GetReference(int i) 
{
	if (i < NSUBTEX )
		return subTex[i];
	else return pblock;
}

void StressTexmap::SetReference(int i, RefTargetHandle rtarg) 
{
	if (i < NSUBTEX)
		subTex[i] = (Texmap *)rtarg; 
	else pblock = (IParamBlock2 *)rtarg; 
}

TSTR StressTexmap::SubAnimName(int i) 
{
	if (i< NSUBTEX)
		return GetSubTexmapTVName(i);
	else return TSTR(_T(""));
}

Animatable* StressTexmap::SubAnim(int i) {
	if (i < NSUBTEX)
		return subTex[i]; 
	else return pblock;
	}

RefResult StressTexmap::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, 
   PartID& partID, RefMessage message ) 
{
	switch (message) {
		case REFMSG_CHANGE:
			ivalid.SetEmpty();
			if (hTarget == pblock)
				{
				ParamID changing_param = pblock->LastNotifyParamID();
				stex_param_blk.InvalidateUI(changing_param);
				}
			break;

		}
	return(REF_SUCCEED);
}



/*===========================================================================*\
 |	Texmap get and set
\*===========================================================================*/

void StressTexmap::SetSubTexmap(int i, Texmap *m) 
{
	ReplaceReference(i,m);
	switch(i) {
		case 0:
			stex_param_blk.InvalidateUI(tex_str_map);
			ivalid.SetEmpty();
			break;
		case 1:
			stex_param_blk.InvalidateUI(tex_rlx_map);
			ivalid.SetEmpty();
			break;
		case 2:
			stex_param_blk.InvalidateUI(tex_cmp_map);
			ivalid.SetEmpty();
			break;
		}
}

TSTR StressTexmap::GetSubTexmapSlotName(int i) {
	switch(i) {
		case 0:  return TSTR(GetString(IDS_STR_MAP)); 
		case 1:  return TSTR(GetString(IDS_RLX_MAP)); 
		default: return TSTR(_T(""));
		}
	}
	 

/*===========================================================================*\
 |	Standard IO
\*===========================================================================*/

#define MTL_HDR_CHUNK 0x4000

IOResult StressTexmap::Save(ISave *isave) { 
	IOResult res;
	isave->BeginChunk(MTL_HDR_CHUNK);
	res = MtlBase::Save(isave);
	if (res!=IO_OK) return res;
	isave->EndChunk();

	return IO_OK;
	}	

IOResult StressTexmap::Load(ILoad *iload) { 
	IOResult res;
	int id;
	while (IO_OK==(res=iload->OpenChunk())) {
		switch(id = iload->CurChunkID())  {
			case MTL_HDR_CHUNK:
				res = MtlBase::Load(iload);
				break;
			}
		iload->CloseChunk();
		if (res!=IO_OK) 
			return res;
		}


	return IO_OK;
	}

/*===========================================================================*\
 |	Updating and cloning
\*===========================================================================*/

RefTargetHandle StressTexmap::Clone(RemapDir &remap) 
{
	StressTexmap *mnew = new StressTexmap();
	*((MtlBase*)mnew) = *((MtlBase*)this);  // copy superclass stuff
	mnew->ReplaceReference(NSUBTEX,remap.CloneRef(pblock));

	mnew->ivalid.SetEmpty();	
	for (int i = 0; i<NSUBTEX; i++) {
		mnew->subTex[i] = NULL;
		if (subTex[i])
			mnew->ReplaceReference(i,remap.CloneRef(subTex[i]));
		mnew->mapOn[i] = mapOn[i];
		}
	return (RefTargetHandle)mnew;
	}

void StressTexmap::NotifyChanged() 
{
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
}

void StressTexmap::Update(TimeValue t, Interval& valid) 
{		
	for(int i=0; i<surfs.Count(); i++) {
		AppDataChunk *ad = surfs[i]->GetAppDataChunk(
			STRESS_TEX_CLASSID, 
			TEXMAP_CLASS_ID, 
			0);
		if(ad) {
			StressData *sd = (StressData*)ad->data;
			sd->DeleteThis();
			surfs[i]->RemoveAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0);
			}
		}
	surfs.SetCount(0);

	if (!ivalid.InInterval(t)) {

		ivalid.SetInfinite();
		pblock->GetValue( tex_str_mapon, t, mapOn[STR], ivalid);
		pblock->GetValue( tex_rlx_mapon, t, mapOn[RLX], ivalid);
		pblock->GetValue( tex_cmp_mapon, t, mapOn[CMP], ivalid);
		pblock->GetValue( tex_mxstress_param, t, maxstress, ivalid);
		pblock->GetValue( tex_mix_param, t, mixval, ivalid);

		pblock->GetValue( tex_canal, t, canal, ivalid);

		pblock->GetValue( tex_show, t, show, ivalid);

		pblock->GetValue( strain_color, t, straincol, ivalid );
		straincol.ClampMinMax();
		pblock->GetValue( relax_color, t, relaxcol, ivalid );
		relaxcol.ClampMinMax();
		pblock->GetValue( comp_color, t, compcol, ivalid );
		compcol.ClampMinMax();

		for (int i=0; i<NSUBTEX; i++) {
			if (subTex[i]) 
				subTex[i]->Update(t,ivalid);
			}
		}
	valid &= ivalid;
	valid = Interval(t,t);
}

int StressTexmap::RenderBegin(TimeValue t, ULONG flags) {
	surfs.SetCount(0);
	return 1;
	}

int StressTexmap::RenderEnd(TimeValue t) {
	for(int i=0; i<surfs.Count(); i++) {
		AppDataChunk *ad = surfs[i]->GetAppDataChunk(
			STRESS_TEX_CLASSID, 
			TEXMAP_CLASS_ID, 
			0);
		if(ad) {
			StressData *sd = (StressData*)ad->data;
			sd->DeleteThis();
			surfs[i]->RemoveAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0);
			}
		}
	surfs.SetCount(0);
	return 1;
	}


