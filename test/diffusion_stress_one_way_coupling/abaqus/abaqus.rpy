# -*- coding: mbcs -*-
#
# Abaqus/CAE Release 6.13-4 replay file
# Internal Version: 2014_01_03-20.01.58 126873
# Run by srinath on Fri Aug 17 14:54:35 2018
#

# from driverUtils import executeOnCaeGraphicsStartup
# executeOnCaeGraphicsStartup()
#: Executing "onCaeGraphicsStartup()" in the site directory ...
from abaqus import *
from abaqusConstants import *
session.Viewport(name='Viewport: 1', origin=(0.0, 0.0), width=488.416046142578, 
    height=246.524993896484)
session.viewports['Viewport: 1'].makeCurrent()
session.viewports['Viewport: 1'].maximize()
from caeModules import *
from driverUtils import executeOnCaeStartup
executeOnCaeStartup()
session.viewports['Viewport: 1'].partDisplay.geometryOptions.setValues(
    referenceRepresentation=ON)
o1 = session.openOdb(
    name='/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2_nondim.odb')
session.viewports['Viewport: 1'].setValues(displayedObject=o1)
#: Model: /new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2_nondim.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       3
#: Number of Node Sets:          3
#: Number of Steps:              1
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].odbDisplay.setPrimaryVariable(
    variableLabel='NT11', outputPosition=NODAL, )
session.Path(name='Path-1', type=NODE_LIST, expression=(('PART-1-1', (11, 137, 
    '599:1061:231', )), ))
xyp = session.XYPlot('XYPlot-1')
chartName = xyp.charts.keys()[0]
chart = xyp.charts[chartName]
pth = session.paths['Path-1']
xy1 = xyPlot.XYDataFromPath(path=pth, includeIntersections=False, 
    pathStyle=PATH_POINTS, numIntervals=10, shape=DEFORMED, 
    labelType=TRUE_DISTANCE)
c1 = session.Curve(xyData=xy1)
chart.setValues(curvesToPlot=(c1, ), )
session.viewports['Viewport: 1'].setValues(displayedObject=xyp)
odb = session.odbs['/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2_nondim.odb']
session.viewports['Viewport: 1'].setValues(displayedObject=odb)
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.odbs['/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2_nondim.odb'].close(
    )
openMdb(
    pathName='/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/heat_transfer2.cae')
#: The model database "/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/heat_transfer2.cae" has been opened.
session.viewports['Viewport: 1'].setValues(displayedObject=None)
p = mdb.models['Model-1'].parts['Part-1']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
session.viewports['Viewport: 1'].partDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].partDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].partDisplay.geometryOptions.setValues(
    referenceRepresentation=OFF)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON, 
    optimizationTasks=OFF, geometricRestrictions=OFF, stopConditions=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
a.makeDependent(instances=(a.instances['Part-1-1'], ))
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
a = mdb.models['Model-1'].rootAssembly
a.makeIndependent(instances=(a.instances['Part-1-1'], ))
session.viewports['Viewport: 1'].assemblyDisplay.setValues(loads=ON, bcs=ON, 
    predefinedFields=ON, connectors=ON)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON, loads=OFF, 
    bcs=OFF, predefinedFields=OFF, connectors=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF, loads=ON, 
    bcs=ON, predefinedFields=ON, connectors=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
mdb.models['Model-1'].predefinedFields['Predefined Field-1'].setValues(
    magnitudes=(1.0, ))
mdb.models['Model-1'].predefinedFields['Predefined Field-1'].setValues(
    magnitudes=(0.0078, ))
session.viewports['Viewport: 1'].assemblyDisplay.setValues(step='Step-1')
mdb.models['Model-1'].loads['Load-1'].setValues(magnitude=1e-06)
session.viewports['Viewport: 1'].partDisplay.setValues(sectionAssignments=ON, 
    engineeringFeatures=ON, mesh=OFF)
session.viewports['Viewport: 1'].partDisplay.meshOptions.setValues(
    meshTechnique=OFF)
p = mdb.models['Model-1'].parts['Part-1']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
mdb.models['Model-1'].materials['Material-1'].conductivity.setValues(table=((
    1e-10, ), ))
session.viewports['Viewport: 1'].partDisplay.setValues(sectionAssignments=OFF, 
    engineeringFeatures=OFF, mesh=ON)
session.viewports['Viewport: 1'].partDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
a.makeDependent(instances=(a.instances['Part-1-1'], ))
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON, loads=OFF, 
    bcs=OFF, predefinedFields=OFF, connectors=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
a = mdb.models['Model-1'].rootAssembly
a.makeIndependent(instances=(a.instances['Part-1-1'], ))
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
elemType1 = mesh.ElemType(elemCode=DC2D4, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=DC2D3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-1-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
a = mdb.models['Model-1'].rootAssembly
e1 = a.instances['Part-1-1'].edges
pickedEdges = e1.getSequenceFromMask(mask=('[#5 ]', ), )
a.seedEdgeByNumber(edges=pickedEdges, number=100, constraint=FINER)
a = mdb.models['Model-1'].rootAssembly
e1 = a.instances['Part-1-1'].edges
pickedEdges = e1.getSequenceFromMask(mask=('[#a ]', ), )
a.seedEdgeByNumber(edges=pickedEdges, number=50, constraint=FINER)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], )
a.generateMesh(regions=partInstances)
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/heat_transfer2.cae".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
mdb.Job(name='ht2', model='Model-1', description='', type=ANALYSIS, 
    atTime=None, waitMinutes=0, waitHours=0, queue=None, memory=90, 
    memoryUnits=PERCENTAGE, getMemoryFromAnalysis=True, 
    explicitPrecision=SINGLE, nodalOutputPrecision=SINGLE, echoPrint=OFF, 
    modelPrint=OFF, contactPrint=OFF, historyPrint=OFF, userSubroutine='', 
    scratch='', multiprocessingMode=DEFAULT, numCpus=1, numGPUs=0)
mdb.jobs['ht2'].submit(consistencyChecking=OFF)
#: The job input file "ht2.inp" has been submitted for analysis.
#: Job ht2: Analysis Input File Processor completed successfully.
o3 = session.openOdb(
    name='/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2.odb')
#: Model: /new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       3
#: Number of Node Sets:          3
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].view.setValues(nearPlane=0.000682948, 
    farPlane=0.00103752, width=0.000309984, height=0.000170644, 
    viewOffsetX=1.51785e-06, viewOffsetY=-1.09909e-06)
session.viewports['Viewport: 1'].odbDisplay.setPrimaryVariable(
    variableLabel='NT11', outputPosition=NODAL, )
session.viewports['Viewport: 1'].view.setValues(nearPlane=0.000683879, 
    farPlane=0.00103659, width=0.000310406, height=0.000170877, 
    viewOffsetX=1.56761e-06, viewOffsetY=-7.67155e-07)
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=0 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=1 )
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(
    adaptiveMeshConstraints=ON)
#: Error in job ht2: Too many increments needed to complete the step
#: Job ht2: Abaqus/Standard aborted due to errors.
mdb.models['Model-1'].steps['Step-1'].setValues(timePeriod=10.0, maxNumInc=10, 
    timeIncrementationMethod=FIXED, initialInc=1.0)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(
    adaptiveMeshConstraints=OFF)
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/heat_transfer2.cae".
mdb.jobs['ht2'].submit(consistencyChecking=OFF)
#: The job input file "ht2.inp" has been submitted for analysis.
#: Traceback (most recent call last):
#:   File "SMAPyaModules/SMAPyaAbqPy.m/src/monitorMechanism.py", line 324, in __processCB
#: xml.parsers.expat.ExpatError: XML or text declaration not at start of entity: line 311, column 0
#: Error in job ht2: XML parsing failure for job ht2.  Shutting down socket and terminating all further messages.  Please check the .log, .dat, .sta, or .msg files for information about the status of the job.
#: Error in job ht2: XML parsing failure for job ht2.  Shutting down socket and terminating all further messages.  Please check the .log, .dat, .sta, or .msg files for information about the status of the job.
#: Job ht2 aborted due to errors.
#: Job ht2 aborted due to errors.
mdb.jobs['ht2'].writeInput(consistencyChecking=OFF)
#: The job input file has been written to "ht2.inp".
mdb.jobs['ht2'].submit(consistencyChecking=OFF)
#: The job input file "ht2.inp" has been submitted for analysis.
#: Job ht2: Analysis Input File Processor completed successfully.
#: Job ht2: Analysis Input File Processor completed successfully.
#: Job ht2: Abaqus/Standard completed successfully.
#: Job ht2: Abaqus/Standard completed successfully.
session.viewports['Viewport: 1'].setValues(displayedObject=None)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
o3 = session.openOdb(
    name='/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2.odb')
#: Model: /new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2.odb
#: Number of Assemblies:         1
#: Number of Assembly instances: 0
#: Number of Part instances:     1
#: Number of Meshes:             1
#: Number of Element Sets:       3
#: Number of Node Sets:          3
#: Number of Steps:              1
session.viewports['Viewport: 1'].setValues(displayedObject=o3)
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].odbDisplay.setPrimaryVariable(
    variableLabel='NT11', outputPosition=NODAL, )
session.viewports['Viewport: 1'].view.setValues(nearPlane=0.000690785, 
    farPlane=0.00102968, width=0.000213594, height=0.000117582, 
    viewOffsetX=-1.73357e-06, viewOffsetY=-4.78245e-06)
session.viewports['Viewport: 1'].odbDisplay.setPrimaryVariable(
    variableLabel='HFL', outputPosition=INTEGRATION_POINT, refinement=(
    INVARIANT, 'Magnitude'), )
session.viewports['Viewport: 1'].view.setValues(nearPlane=0.000688286, 
    farPlane=0.00103218, width=0.000254108, height=0.000139885, 
    viewOffsetX=-1.72436e-06, viewOffsetY=-1.13324e-05)
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=0 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=1 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=2 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=1 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=2 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=3 )
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=4 )
session.viewports['Viewport: 1'].odbDisplay.setPrimaryVariable(
    variableLabel='NT11', outputPosition=NODAL, )
del session.paths['Path-1']
session.viewports['Viewport: 1'].view.setValues(nearPlane=0.000693672, 
    farPlane=0.00102679, width=0.000181655, height=0.0001, 
    viewOffsetX=-8.21073e-07, viewOffsetY=-1.0154e-05)
session.Path(name='Path-1', type=NODE_LIST, expression=(('PART-1-1', (
    '51:253:101', 455, 657, 960, 1162, 2980, 4091, 5101, )), ))
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=10)
session.viewports['Viewport: 1'].odbDisplay.setFrame(step=0, frame=10)
xyp = session.xyPlots['XYPlot-1']
chartName = xyp.charts.keys()[0]
chart = xyp.charts[chartName]
pth = session.paths['Path-1']
xy1 = xyPlot.XYDataFromPath(path=pth, includeIntersections=False, 
    pathStyle=UNIFORM_SPACING, numIntervals=20, shape=DEFORMED, 
    labelType=TRUE_DISTANCE)
c1 = session.Curve(xyData=xy1)
chart.setValues(curvesToPlot=(c1, ), )
session.viewports['Viewport: 1'].setValues(displayedObject=xyp)
xyp = session.xyPlots['XYPlot-1']
chartName = xyp.charts.keys()[0]
chart = xyp.charts[chartName]
pth = session.paths['Path-1']
xy1 = xyPlot.XYDataFromPath(path=pth, includeIntersections=True, 
    pathStyle=PATH_POINTS, numIntervals=20, shape=DEFORMED, 
    labelType=TRUE_DISTANCE)
c1 = session.Curve(xyData=xy1)
chart.setValues(curvesToPlot=(c1, ), )
#* VisError: No results could be found for xy data extraction from path.
odb = session.odbs['/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2.odb']
session.viewports['Viewport: 1'].setValues(displayedObject=odb)
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
xyp = session.xyPlots['XYPlot-1']
chartName = xyp.charts.keys()[0]
chart = xyp.charts[chartName]
pth = session.paths['Path-1']
xy1 = xyPlot.XYDataFromPath(path=pth, includeIntersections=True, 
    pathStyle=PATH_POINTS, numIntervals=20, shape=DEFORMED, 
    labelType=TRUE_DISTANCE)
c1 = session.Curve(xyData=xy1)
chart.setValues(curvesToPlot=(c1, ), )
session.viewports['Viewport: 1'].setValues(displayedObject=xyp)
#* VisError: No results could be found for xy data extraction from path.
xyp = session.xyPlots['XYPlot-1']
chartName = xyp.charts.keys()[0]
chart = xyp.charts[chartName]
pth = session.paths['Path-1']
xy1 = xyPlot.XYDataFromPath(path=pth, includeIntersections=False, 
    pathStyle=PATH_POINTS, numIntervals=20, shape=DEFORMED, 
    labelType=TRUE_DISTANCE)
c1 = session.Curve(xyData=xy1)
chart.setValues(curvesToPlot=(c1, ), )
session.viewports['Viewport: 1'].setValues(displayedObject=xyp)
odb = session.odbs['/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/ht2.odb']
session.viewports['Viewport: 1'].setValues(displayedObject=odb)
session.viewports['Viewport: 1'].odbDisplay.display.setValues(plotState=(
    CONTOURS_ON_DEF, ))
session.viewports['Viewport: 1'].view.setValues(nearPlane=0.00067991, 
    farPlane=0.00104056, width=0.000346804, height=0.000190914, 
    viewOffsetX=4.66622e-05, viewOffsetY=4.06129e-06)
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/diffusion_stress_one_way_coupling/abaqus/heat_transfer2.cae".
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
mdb.jobs['ht2'].kill()
