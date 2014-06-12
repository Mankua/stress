/*===========================================================================*\
 | 
 |  FILE:	Texmap.h
 |			Skeleton project and code for a Texture Map
 |			3D Studio MAX R3.0
 | 
 |  AUTH:   Diego Castaño
 |			Mankua
 |
 |  HIST:	Started 11-3-99
 | 
\*===========================================================================*/

#ifndef __TEXSKEL__H
#define __TEXSKEL__H

#include "max.h"
#include "imtl.h"
#include "texutil.h"
#include "iparamm2.h"
#include "resource.h"

// IMPORTANT:
// The ClassID must be changed whenever a new project
// is created using this skeleton
#define	STRESS_TEX_CLASSID		Class_ID(0x22a453a, 0x730e5338)


TCHAR *GetString(int id);
extern ClassDesc* GetStressTexmapDesc();


// Paramblock2 name
enum { tex_params, }; 
// Paramblock2 parameter list
enum { tex_mxstress_param, 
	   tex_str_map, tex_str_mapon,
	   tex_rlx_map, tex_rlx_mapon,
	   tex_cmp_map, tex_cmp_mapon,
	   tex_canal,
	   strain_color, relax_color, comp_color, tex_mix_param, tex_show};


// How many subtex's we support
#define NSUBTEX		3
#define PBLOCK_REF	3

#define STR			0
#define RLX			1
#define CMP			2

#define SHOW_STRAIN	0
#define SHOW_COMP	1
#define SHOW_BOTH	2

/*===========================================================================*\
 |	StressData class defn
\*===========================================================================*/

class StressData {
	public:
		int numverts;
		float * data;
		float * strain;
		float * comp;

		StressData() {
			numverts = 0;
			data = NULL;
			strain = NULL;
			comp = NULL;
			}

		~StressData() {
			if (data) {
				delete [] data;
				data = NULL;
				}
			if (strain) {
				delete [] strain;
				strain = NULL;
				}
			if (comp) {
				delete [] comp;
				comp = NULL;
				}
			numverts = 0;
			delete this;
			}

		void DeleteThis() {
			if (data) {
				delete [] data;
				data = NULL;
				}
			if (strain) {
				delete [] strain;
				strain = NULL;
				}
			if (comp) {
				delete [] comp;
				comp = NULL;
				}
			numverts = 0;
			}

		void SetNumVerts(int nv) {
			if (data) {
				delete [] data;
				data = NULL;
				}
			if (strain) {
				delete [] strain;
				strain = NULL;
				}
			if (comp) {
				delete [] comp;
				comp = NULL;
				}

			data = new float[nv];
			comp = new float[nv];
			strain = new float[nv];
			numverts = nv;

			for (int i=0; i<numverts; i++) {
				data[i] = 0.0f;
				strain[i] = 0.0f;
				comp[i] = 0.0f;
				}
			}

		float GetData(int vert) { return data[vert]; }
		void SetData(int vert, float F) { data[vert] = F; }
		void AddData(int vert, float F) { data[vert] += F; }

		float GetStrain(int vert) { return strain[vert]; }
		void SetStrain(int vert, float F) { if (F >= 0.0f) strain[vert] = F; }
		void AddStrain(int vert, float F) { if (F >= 0.0f) strain[vert] += F; }

		float GetComp(int vert) { return comp[vert]; }
		void SetComp(int vert, float F) { if (F <= 0.0f) comp[vert] = F; }
		void AddComp(int vert, float F) { if (F <= 0.0f) comp[vert] += F; }
	};

/*===========================================================================*\
 |	StressTexmap class defn
\*===========================================================================*/

class StressTexmap: public Texmap 
{ 
	public:
		Interval ivalid;
		Texmap* subTex[NSUBTEX];
		int canal;

		float Fmax;		// Valor de referencia para el calculo del color.
		Color straincol;
		Color relaxcol;
		Color compcol;
		int show;

		// various variables
		float maxstress;
		float mixval;

		BOOL mapOn[NSUBTEX];

		Tab <INode*> surfs;		// Tabla con los objetos que tienen aplicado el aatmap


		StressTexmap();
		~StressTexmap();

		// This plugin needs to be Thread Safe, so...
		CRITICAL_SECTION csect;

		// Parameter and UI management
		IParamBlock2 *pblock;
		ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
		void Update(TimeValue t, Interval& valid);
		int RenderBegin(TimeValue t, ULONG flags);
		int RenderEnd(TimeValue t);

		void Reset();
		Interval Validity(TimeValue t) { Interval v; Update(t,v); return ivalid; }

		void NotifyChanged();

		// Evaluate Stress
		void SetStress(ShadeContext &sc);

		// Evaluate the color of map for the context.
		AColor EvalColor(ShadeContext& sc);
		float EvalMono(ShadeContext& sc);
		AColor EvalFunction(ShadeContext& sc, float u, float v, float du, float dv);

		// For Bump mapping, need a perturbation to apply to a normal.
		Point3 EvalNormalPerturb(ShadeContext& sc);

		// Methods to access texture maps of material
		int NumSubTexmaps() { return NSUBTEX; }
		Texmap* GetSubTexmap(int i) { return subTex[i]; }
		void SetSubTexmap(int i, Texmap *m);
		TSTR GetSubTexmapSlotName(int i);

		Class_ID ClassID() {	return STRESS_TEX_CLASSID; }
		SClass_ID SuperClassID() { return TEXMAP_CLASS_ID; }
		void GetClassName(TSTR& s) { s= GetString(IDS_CLASSNAME); }  
		void DeleteThis() { delete this; }	

		int NumSubs() { return NSUBTEX+1; }  
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);
		int SubNumToRefNum(int subNum) { return subNum; }

		// From ref
 		int NumRefs() { return NSUBTEX+1; }
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);

#if ( MAX_RELEASE < 9000 ) 
		RefTargetHandle Clone(RemapDir &remap = NoRemap());
#else
		RefTargetHandle Clone(RemapDir &remap=DefaultRemapDir());
#endif

		RefResult NotifyRefChanged( Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message );

		// IO
		IOResult Save(ISave *isave);
		IOResult Load(ILoad *iload);

		int	NumParamBlocks() { return 1; }				
		IParamBlock2* GetParamBlock(int i) { return pblock; }
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } 
};


/*===========================================================================*\
 |	Dialog Processor
\*===========================================================================*/

class SkelTexDlgProc : public ParamMap2UserDlgProc 
{
	public:
		IParamMap *pmap;
		StressTexmap *st;

		SkelTexDlgProc(StressTexmap *m) {st = m;}		
		INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void DeleteThis() { }
		void SetThing(ReferenceTarget *m) {
			st = (StressTexmap*)m;
			}

		void InitAuthorized(HWND hWnd);
		void ShowStressControls(HWND hPanel, BOOL show);
		void ShowAuthoControls(HWND hPanel, BOOL show);
};



#endif