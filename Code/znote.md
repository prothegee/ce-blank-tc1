# znote

---

<br>

## I3DEngine
### https://docs.cryengine.com/display/CPP/I3DEngine
```
useful CVars

e_Bboxex
e_DebugDraw
e_CheckOcclusion
e_CoverageBufferDebug
sys_MaxFps // only works when r_sync=0

e_Shadows
ca_DrawCHR
e_Entities
e_Brushes
e_Vegetation
e_Sun
e_WaterVolumes
e_WaterOcean

r_AntialiasingMode
r_AntialiasingModeDebug
```

---

<br>

## rendering API layers

__hight level API:__
- CSceneRenderPass
- CPrimitiveRenderPass
- CFullScreenPass
- CComputeRenderPass

- CTexture
- CGPUBuffer
- CConstantBuffer
- buffer_handle_t
- CShader
- SamplerStateHandle

__low level API (DeviceLayer):__
- CDeviceBufferManager

- CDeviceResourceSet
- CDeviceCommandList
- CDeviceResourceLayout
- CDeviceComputePSO
- CDeviceStreamSet
- CDeviceGraphicsPSO
- CDeviceObjectFactory

__native layer (implementation of DeviceLayer):__
- DirectX 11:
    * CDeviceResourceSet_DX11
    * CDeviceCommandList_DX11

- DirectX 12:
    * CDeviceResourceSet_DX12
    * CDeviceCommandList_DX12

- GNM:
    * GnmResourceSet
    * GnmCommandList



---

<br>

###### end of znote
