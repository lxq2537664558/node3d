#include "UIDisplayModel.h"
#include "RenderSystem.h"
#include "UIDialog.h"
#include "Graphics.h"
#include "SkeletonNode.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CUIDisplayRenderNode::CUIDisplayRenderNode()
	:m_pRenderNode(NULL)
	,m_vEye(0.0f, 0.0f, -1.0f)
	,m_vLookAt(0.0f, 0.0f, 0.0f)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CUIDisplayRenderNode::~CUIDisplayRenderNode()
{
	delete m_pRenderNode;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRenderNode::LoadModel(const char * szFilename)
{
	S_DEL(m_pRenderNode);
	// ----
	m_pRenderNode = new CSkeletonNode;
	m_pRenderNode->load(szFilename);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRenderNode::OnFrameMove(double fTime, float fElapsedTime)
{
	// ----
	if(m_pRenderNode)
	{
		m_pRenderNode->frameMove(Matrix::UNIT, fTime, fElapsedTime);
	}
	// ----
	CUIDisplay::OnFrameMove(fTime, fElapsedTime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRenderNode::OnFrameRender(const Matrix & mTransform, double fTime, float fElapsedTime)
{
	// ----
	if(m_pRenderNode && (IsVisible() == true) && (isStyleVisible() == true) && GetParentDialog()->IsVisible())
	{
		Vec3D vLightDir(-0.274226f,-0.68f,0.68f);
		// ----
		CRenderSystem & R = CRenderSystem::getSingleton();
		// ----
		R.setShaderFloat("g_fTime",				fTime);
		R.setShaderFloatArray("g_vLightDir",	&vLightDir, 3);
		// ----
		// Light Matrix
		// 		const Vec3D& vEyePoint = CPlayerMe::getInstance().getPos();
		// 		BBox lightBox(vEyePoint.x-10,vEyePoint.y-10,vEyePoint.z-10,
		// 			vEyePoint.x+10,vEyePoint.y+10,vEyePoint.z+10);
		// 		Matrix mLight = calcLightMatrix(lightBox, vLightDir);
		// 		R.setShaderMatrix("lvm", mLight);
		// ----
		R.setProjectionMatrix(m_mProj);
		R.setViewMatrix(m_mView);
		R.setWorldMatrix(Matrix::UNIT);
		// ----
		// # Render World
		// ----
		m_SceneEffect.render(m_pRenderNode);
		// ----
		R.setViewport(GetParentDialog()->GetBoundingBox());
		// ----
	}
	CUIDisplay::OnFrameRender(mTransform, fTime, fElapsedTime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRenderNode::OnSize(const CRect<int>& rc)
{
	CUIDisplay::OnSize(rc);
	// ----
	m_SceneEffect.Reset(GetBoundingBox());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------