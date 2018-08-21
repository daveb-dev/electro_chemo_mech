#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# create a new 'ExodusIIReader'
chemo_mech_si_2d_oute = ExodusIIReader(FileName=['/home/srinath/Documents/workspace_dev/electro_chemo_mech/test/diffusion_stress_one_way_coupling/chemo_mech_si_2d_out.e'])
chemo_mech_si_2d_oute.ElementVariables = []
chemo_mech_si_2d_oute.PointVariables = []
chemo_mech_si_2d_oute.NodeSetArrayStatus = []
chemo_mech_si_2d_oute.SideSetArrayStatus = []

# get animation scene
animationScene1 = GetAnimationScene()

# update animation scene based on data timesteps
animationScene1.UpdateAnimationUsingDataTimeSteps()

# Properties modified on chemo_mech_si_2d_oute
chemo_mech_si_2d_oute.ElementVariables = ['stress_11', 'stress_22', 'stress_33']
chemo_mech_si_2d_oute.PointVariables = ['conc', 'disp_']
chemo_mech_si_2d_oute.ElementBlocks = ['Unnamed block ID: 0 Type: HEX8']

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [1443, 759]

# show data in view
chemo_mech_si_2d_outeDisplay = Show(chemo_mech_si_2d_oute, renderView1)
# trace defaults for the display properties.
chemo_mech_si_2d_outeDisplay.ColorArrayName = [None, '']
chemo_mech_si_2d_outeDisplay.GlyphType = 'Arrow'
chemo_mech_si_2d_outeDisplay.ScalarOpacityUnitDistance = 1.2151377801118142

# reset view to fit data
renderView1.ResetCamera()

# set scalar coloring
ColorBy(chemo_mech_si_2d_outeDisplay, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
chemo_mech_si_2d_outeDisplay.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'vtkBlockColors'
vtkBlockColorsLUT = GetColorTransferFunction('vtkBlockColors')

# get opacity transfer function/opacity map for 'vtkBlockColors'
vtkBlockColorsPWF = GetOpacityTransferFunction('vtkBlockColors')

# create a new 'Calculator'
calculator1 = Calculator(Input=chemo_mech_si_2d_oute)
calculator1.Function = ''

# Properties modified on calculator1
calculator1.ResultArrayName = 'disp_vector'
calculator1.Function = 'iHat*disp__X + jHat*disp__Y + kHat*disp__Z'

# show data in view
calculator1Display = Show(calculator1, renderView1)
# trace defaults for the display properties.
calculator1Display.ColorArrayName = [None, '']
calculator1Display.GlyphType = 'Arrow'
calculator1Display.ScalarOpacityUnitDistance = 1.2151377801118142

# hide data in view
Hide(chemo_mech_si_2d_oute, renderView1)

# set scalar coloring
ColorBy(calculator1Display, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
calculator1Display.SetScalarBarVisibility(renderView1, True)

# create a new 'Warp By Vector'
warpByVector1 = WarpByVector(Input=calculator1)
warpByVector1.Vectors = ['POINTS', 'disp_vector']

# Properties modified on warpByVector1
warpByVector1.ScaleFactor = 10000000.0

# show data in view
warpByVector1Display = Show(warpByVector1, renderView1)
# trace defaults for the display properties.
warpByVector1Display.ColorArrayName = [None, '']
warpByVector1Display.GlyphType = 'Arrow'
warpByVector1Display.ScalarOpacityUnitDistance = 1.2151377801118142

# hide data in view
Hide(calculator1, renderView1)

# set scalar coloring
ColorBy(warpByVector1Display, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
warpByVector1Display.SetScalarBarVisibility(renderView1, True)

# set scalar coloring
ColorBy(warpByVector1Display, ('POINTS', 'conc'))

# rescale color and/or opacity maps used to include current data range
warpByVector1Display.RescaleTransferFunctionToDataRange(True)

# show color bar/color legend
warpByVector1Display.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'conc'
concLUT = GetColorTransferFunction('conc')

# get opacity transfer function/opacity map for 'conc'
concPWF = GetOpacityTransferFunction('conc')

# change representation type
warpByVector1Display.SetRepresentationType('Surface With Edges')

# rescale color and/or opacity maps used to exactly fit the current data range
warpByVector1Display.RescaleTransferFunctionToDataRange(False)

#### saving camera placements for all active views

# current camera placement for renderView1
renderView1.CameraPosition = [10.0, 5.0, 43.74069211509001]
renderView1.CameraFocalPoint = [10.0, 5.0, 0.5]
renderView1.CameraParallelScale = 11.191514642799696

#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).