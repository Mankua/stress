/*===========================================================================*\
 | 
 |  FILE:	TexShade.cpp
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

static AColor white(1.0f,1.0f,1.0f,1.0f);


void StressTexmap::SetStress(ShadeContext &sc) {
	int i;

	StressData *sd = new StressData;
	Mesh *mi = sc.globContext->GetRenderInstance(sc.NodeID())->mesh;

	if (!mi->mapSupport(canal)) {
		sd->SetNumVerts(mi->getNumMapVerts(canal));
		int len = sd->numverts * sizeof(float) * 3.0f + sizeof(int);
		sc.Node()->RemoveAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0);
		sc.Node()->AddAppDataChunk(STRESS_TEX_CLASSID,TEXMAP_CLASS_ID,0,(DWORD)len,sd);
		return;
		}

	UVVert *mv = mi->mapVerts(canal);
	TVFace *mf = mi->mapFaces(canal);

	Tab <int> refct;
	sd->SetNumVerts(mi->getNumMapVerts(canal));
	refct.SetCount(mi->getNumMapVerts(canal));
	for (i=0; i<mi->getNumMapVerts(canal); i++) refct[i] = 0;

	for (i=0; i<mi->numFaces; i++) {
		Point3 vi0 = mv[mf[i].getTVert(0)];
		Point3 vi1 = mv[mf[i].getTVert(1)];
		Point3 vi2 = mv[mf[i].getTVert(2)];

		Point3 vf0 = mi->verts[mi->faces[i].getVert(0)];
		Point3 vf1 = mi->verts[mi->faces[i].getVert(1)];
		Point3 vf2 = mi->verts[mi->faces[i].getVert(2)];

		float leni,lenf,F;

		// Fuerza entre v0 y v1
		if (mi->faces[i].flags&EDGE_A) {
			leni	= Length(vi0 - vi1);
			lenf	= Length(vf0 - vf1);

			if (lenf >= leni)	F =  (lenf/leni - 1.0f);
			else				F = -(leni/lenf - 1.0f);

			refct[mf[i].getTVert(0)]++;
			refct[mf[i].getTVert(1)]++;

			sd->AddData(mf[i].getTVert(0),F);
			sd->AddData(mf[i].getTVert(1),F);
			sd->AddComp(mf[i].getTVert(0),F);
			sd->AddComp(mf[i].getTVert(1),F);
			sd->AddStrain(mf[i].getTVert(0),F);
			sd->AddStrain(mf[i].getTVert(1),F);
			}


		// Fuerza entre v1 y v2
		if (mi->faces[i].flags&EDGE_B) {
			leni	= Length(vi1 - vi2);
			lenf	= Length(vf1 - vf2);

		//	Original Physical model
		//	dlen	= leni - lenf;
		//	if (dlen>=0)	fdir = -1.0f;
		//	else			fdir =  1.0f;
		//	F		= fdir * 0.5f * dlen * dlen;

			if (lenf >= leni)	F =  (lenf/leni - 1.0f);
			else				F = -(leni/lenf - 1.0f);
			
			refct[mf[i].getTVert(1)]++;
			refct[mf[i].getTVert(2)]++;

			sd->AddData(mf[i].getTVert(1),F);
			sd->AddData(mf[i].getTVert(2),F);
			sd->AddComp(mf[i].getTVert(1),F);
			sd->AddComp(mf[i].getTVert(2),F);
			sd->AddStrain(mf[i].getTVert(1),F);
			sd->AddStrain(mf[i].getTVert(2),F);
			}

		// Fuerza entre v2 y v0
		if (mi->faces[i].flags&EDGE_C) {
			leni	= Length(vi2 - vi0);
			lenf	= Length(vf2 - vf0);

			if (lenf >= leni)	F =  (lenf/leni - 1.0f);
			else				F = -(leni/lenf - 1.0f);

			refct[mf[i].getTVert(2)]++;
			refct[mf[i].getTVert(0)]++;

			sd->AddData(mf[i].getTVert(2),F);
			sd->AddData(mf[i].getTVert(0),F);
			sd->AddComp(mf[i].getTVert(2),F);
			sd->AddComp(mf[i].getTVert(0),F);
			sd->AddStrain(mf[i].getTVert(2),F);
			sd->AddStrain(mf[i].getTVert(0),F);
			}
		}

	Fmax = maxstress / 100.0f;

	float F;
	for (i=0; i<sd->numverts; i++) {
		// Promedio la sumatoria de Fuerzas entre el numero de
		// ejes vivos de este vertice.
		F = sd->GetData(i) / (float)refct[i];

		if (F >  Fmax) F =  Fmax; 
		if (F < -Fmax) F = -Fmax;

		F = (F / (2.0f*Fmax)) + 0.5f;

		if (F<=0.5f) F = pow(2.0f * F,mixval) / 2.0f;
		else		 F = 1.0f - pow(2.0f - 2.0f * F, mixval) / 2.0f;

		F = (F - 0.5f) * 2.0f * Fmax;
		sd->SetData(i,F);

		F = sd->GetComp(i) / (float)refct[i];
		if (F >  Fmax) F =  Fmax; 
		if (F < -Fmax) F = -Fmax;

		F = (F / (2.0f*Fmax)) + 0.5f;

		if (F<=0.5f) F = pow(2.0f * F,mixval) / 2.0f;
		else		 F = 1.0f - pow(2.0f - 2.0f * F, mixval) / 2.0f;

		F = (F - 0.5f) * 2.0f * Fmax;
		sd->SetComp(i,F);

		F = sd->GetStrain(i) / (float)refct[i];
		if (F >  Fmax) F =  Fmax; 
		if (F < -Fmax) F = -Fmax;

		F = (F / (2.0f*Fmax)) + 0.5f;

		if (F<=0.5f) F = pow(2.0f * F,mixval) / 2.0f;
		else		 F = 1.0f - pow(2.0f - 2.0f * F, mixval) / 2.0f;

		F = (F - 0.5f) * 2.0f * Fmax;
		sd->SetStrain(i,F);
		}

	int len = sd->numverts * sizeof(float) * 3.0f + sizeof(int);
					
	sc.Node()->RemoveAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0);

	sc.Node()->AddAppDataChunk(STRESS_TEX_CLASSID,TEXMAP_CLASS_ID,0,(DWORD)len,sd);
	}

/*===========================================================================*\
 |	Actual shading takes place
\*===========================================================================*/

AColor StressTexmap::EvalColor(ShadeContext& sc) 
{
	if (gbufID) sc.SetGBufferID(gbufID);
	AColor c,cr,cs,cc;

	if (sc.InMtlEditor()) {
		c = subTex[RLX]&&mapOn[RLX]? subTex[RLX]->EvalColor(sc): AColor(relaxcol);;
		}
	else {
		if(!sc.Node()->GetAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0)) {
			EnterCriticalSection(&csect);
			if(!sc.Node()->GetAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0)) {
				SetStress(sc);
				surfs.SetCount(surfs.Count()+1);
				surfs[surfs.Count()-1] = sc.Node();
				}
			LeaveCriticalSection(&csect);
			}

		AppDataChunk *ad = sc.Node()->GetAppDataChunk(STRESS_TEX_CLASSID,TEXMAP_CLASS_ID,0);
		if (ad == NULL) return AColor(0.0f,0.0f,0.0f,0.0f);
		StressData *sd = (StressData*)ad->data;


		Mesh *mi = sc.globContext->GetRenderInstance(sc.NodeID())->mesh;
		if (show == SHOW_BOTH) {
			float f0 = sd->GetData(mi->faces[sc.FaceNumber()].getVert(0));
			float f1 = sd->GetData(mi->faces[sc.FaceNumber()].getVert(1));
			float f2 = sd->GetData(mi->faces[sc.FaceNumber()].getVert(2));

			float f =	f0*sc.BarycentricCoords().x + 
						f1*sc.BarycentricCoords().y +
						f2*sc.BarycentricCoords().z;

			cr = mapOn[RLX]&&subTex[RLX] ? subTex[RLX]->EvalColor(sc): AColor(relaxcol);	
			if (f == 0.0f) 
				c = cr;
			else if (f > 0.0f) {
				float d = f/Fmax;
				cs = mapOn[STR]&&subTex[STR] ? subTex[STR]->EvalColor(sc): AColor(straincol);	
				c = cr * (1.0f-d) + cs*d;
				}
			else {
				float d = -f/Fmax;
				cc = mapOn[CMP]&&subTex[CMP] ? subTex[CMP]->EvalColor(sc): AColor(compcol);
				c = cr * (1.0f-d) + cc * d;
				}
			}
		else if (show == SHOW_COMP) {
			float f0 = sd->GetComp(mi->faces[sc.FaceNumber()].getVert(0));
			float f1 = sd->GetComp(mi->faces[sc.FaceNumber()].getVert(1));
			float f2 = sd->GetComp(mi->faces[sc.FaceNumber()].getVert(2));

			float f =	f0*sc.BarycentricCoords().x + 
						f1*sc.BarycentricCoords().y +
						f2*sc.BarycentricCoords().z;

			cr = mapOn[RLX]&&subTex[RLX] ? subTex[RLX]->EvalColor(sc): AColor(relaxcol);	
			if (f == 0.0f) 
				c = cr;
			else {
				float d = -f/Fmax;
				cc = mapOn[CMP]&&subTex[CMP] ? subTex[CMP]->EvalColor(sc): AColor(compcol);
				c = cr * (1.0f-d) + cc * d;
				}
			}
		else {
			float f0 = sd->GetStrain(mi->faces[sc.FaceNumber()].getVert(0));
			float f1 = sd->GetStrain(mi->faces[sc.FaceNumber()].getVert(1));
			float f2 = sd->GetStrain(mi->faces[sc.FaceNumber()].getVert(2));

			float f =	f0*sc.BarycentricCoords().x + 
						f1*sc.BarycentricCoords().y +
						f2*sc.BarycentricCoords().z;

			cr = mapOn[RLX]&&subTex[RLX] ? subTex[RLX]->EvalColor(sc): AColor(relaxcol);	
			if (f == 0.0f) 
				c = cr;
			else {
				float d = f/Fmax;
				cs = mapOn[STR]&&subTex[STR] ? subTex[STR]->EvalColor(sc): AColor(straincol);	
				c = cr * (1.0f-d) + cs * d;
				}
			}
		}

	return c;
}

float StressTexmap::EvalMono(ShadeContext& sc) {
	IPoint2 xy = sc.ScreenCoord();
	return Intens(EvalColor(sc));
	}

Point3 StressTexmap::EvalNormalPerturb(ShadeContext& sc) 
{
	if (gbufID) sc.SetGBufferID(gbufID);
	Point3 p,pr,ps,pc;

	if (sc.InMtlEditor()) {
		p = subTex[RLX]&&mapOn[RLX]? subTex[RLX]->EvalNormalPerturb(sc): Point3(0,0,0);
		}
	else {
		if(!sc.Node()->GetAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0)) {
			EnterCriticalSection(&csect);
			if(!sc.Node()->GetAppDataChunk(STRESS_TEX_CLASSID, TEXMAP_CLASS_ID, 0)) {
				SetStress(sc);
				surfs.SetCount(surfs.Count()+1);
				surfs[surfs.Count()-1] = sc.Node();
				}
			LeaveCriticalSection(&csect);
			}

		AppDataChunk *ad = sc.Node()->GetAppDataChunk(STRESS_TEX_CLASSID,TEXMAP_CLASS_ID,0);
		if (ad == NULL) return Point3(0,0,0);
		StressData *sd = (StressData*)ad->data;


		Mesh *mi = sc.globContext->GetRenderInstance(sc.NodeID())->mesh;
		if (show == SHOW_BOTH) {
			float f0 = sd->GetData(mi->faces[sc.FaceNumber()].getVert(0));
			float f1 = sd->GetData(mi->faces[sc.FaceNumber()].getVert(1));
			float f2 = sd->GetData(mi->faces[sc.FaceNumber()].getVert(2));

			float f =	f0*sc.BarycentricCoords().x + 
						f1*sc.BarycentricCoords().y +
						f2*sc.BarycentricCoords().z;

			pr = mapOn[RLX]&&subTex[RLX] ? subTex[RLX]->EvalNormalPerturb(sc): Point3(0,0,0);
			if (f == 0.0f) 
				p = pr;
			else if (f > 0.0f) {
				float d = f/Fmax;
				ps = mapOn[STR]&&subTex[STR] ? subTex[STR]->EvalNormalPerturb(sc): Point3(0,0,0);
				p = (1.0f-d) * pr + d * ps;
				}
			else {
				float d = -f/Fmax;
				pc = mapOn[CMP]&&subTex[CMP] ? subTex[CMP]->EvalNormalPerturb(sc): Point3(0,0,0);
				p = (1.0f-d) * pr + d * pc;
				}
			}
		else if (show == SHOW_COMP) {
			float f0 = sd->GetComp(mi->faces[sc.FaceNumber()].getVert(0));
			float f1 = sd->GetComp(mi->faces[sc.FaceNumber()].getVert(1));
			float f2 = sd->GetComp(mi->faces[sc.FaceNumber()].getVert(2));

			float f =	f0*sc.BarycentricCoords().x + 
						f1*sc.BarycentricCoords().y +
						f2*sc.BarycentricCoords().z;

			pr = mapOn[RLX]&&subTex[RLX] ? subTex[RLX]->EvalNormalPerturb(sc): Point3(0,0,0);
			if (f == 0.0f) 
				p = pr;
			else {
				float d = -f/Fmax;
				pc = mapOn[CMP]&&subTex[CMP] ? subTex[CMP]->EvalNormalPerturb(sc): Point3(0,0,0);
				p = (1.0f-d) * pr + d * pc;
				}
			}
		else {
			float f0 = sd->GetStrain(mi->faces[sc.FaceNumber()].getVert(0));
			float f1 = sd->GetStrain(mi->faces[sc.FaceNumber()].getVert(1));
			float f2 = sd->GetStrain(mi->faces[sc.FaceNumber()].getVert(2));

			float f =	f0*sc.BarycentricCoords().x + 
						f1*sc.BarycentricCoords().y +
						f2*sc.BarycentricCoords().z;

			pr = mapOn[RLX]&&subTex[RLX] ? subTex[RLX]->EvalNormalPerturb(sc): Point3(0,0,0);
			if (f == 0.0f) 
				p = pr;
			else {
				float d = f/Fmax;
				ps = mapOn[STR]&&subTex[STR] ? subTex[STR]->EvalNormalPerturb(sc): Point3(0,0,0);
				p = (1.0f-d) * pr + d * ps;
				}
			}
		}

	return p;
}
