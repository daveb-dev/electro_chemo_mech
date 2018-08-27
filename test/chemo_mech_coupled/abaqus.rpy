# -*- coding: mbcs -*-
#
# Abaqus/CAE Release 6.13-4 replay file
# Internal Version: 2014_01_03-20.01.58 126873
# Run by srinath on Sat Aug 25 10:50:50 2018
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
s = mdb.models['Model-1'].ConstrainedSketch(name='__profile__', 
    sheetSize=200.0)
g, v, d, c = s.geometry, s.vertices, s.dimensions, s.constraints
s.setPrimaryObject(option=STANDALONE)
s.rectangle(point1=(-3.0, -3.0), point2=(3.0, 3.0))
session.viewports['Viewport: 1'].view.setValues(nearPlane=185.085, 
    farPlane=192.039, width=44.4736, height=24.4825, cameraPosition=(3.71383, 
    0.570467, 188.562), cameraTarget=(3.71383, 0.570467, 0))
s.unsetPrimaryObject()
del mdb.models['Model-1'].sketches['__profile__']
s1 = mdb.models['Model-1'].ConstrainedSketch(name='__profile__', 
    sheetSize=200.0)
g, v, d, c = s1.geometry, s1.vertices, s1.dimensions, s1.constraints
s1.setPrimaryObject(option=STANDALONE)
s1.rectangle(point1=(-3.0, -3.0), point2=(3.0, 3.0))
s1.CircleByCenterPerimeter(center=(0.0, 0.0), point1=(1.5, 0.0))
session.viewports['Viewport: 1'].view.setValues(nearPlane=186.332, 
    farPlane=190.792, width=28.5271, height=15.704, cameraPosition=(3.81129, 
    0.304931, 188.562), cameraTarget=(3.81129, 0.304931, 0))
p = mdb.models['Model-1'].Part(name='Part-1', dimensionality=TWO_D_PLANAR, 
    type=DEFORMABLE_BODY)
p = mdb.models['Model-1'].parts['Part-1']
p.BaseShell(sketch=s1)
s1.unsetPrimaryObject()
p = mdb.models['Model-1'].parts['Part-1']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
del mdb.models['Model-1'].sketches['__profile__']
session.viewports['Viewport: 1'].view.setValues(nearPlane=15.6051, 
    farPlane=18.3361, width=17.4384, height=9.59972, viewOffsetX=1.54319, 
    viewOffsetY=-0.111416)
s = mdb.models['Model-1'].ConstrainedSketch(name='__profile__', 
    sheetSize=200.0)
g, v, d, c = s.geometry, s.vertices, s.dimensions, s.constraints
s.setPrimaryObject(option=STANDALONE)
s.CircleByCenterPerimeter(center=(0.0, 0.0), point1=(1.5, 0.0))
p = mdb.models['Model-1'].Part(name='Part-2', dimensionality=TWO_D_PLANAR, 
    type=DEFORMABLE_BODY)
p = mdb.models['Model-1'].parts['Part-2']
p.BaseShell(sketch=s)
s.unsetPrimaryObject()
p = mdb.models['Model-1'].parts['Part-2']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
del mdb.models['Model-1'].sketches['__profile__']
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(
    optimizationTasks=OFF, geometricRestrictions=OFF, stopConditions=OFF)
session.viewports['Viewport: 1'].partDisplay.setValues(sectionAssignments=ON, 
    engineeringFeatures=ON)
session.viewports['Viewport: 1'].partDisplay.geometryOptions.setValues(
    referenceRepresentation=OFF)
p = mdb.models['Model-1'].parts['Part-2']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
mdb.models['Model-1'].Material(name='Material-1')
mdb.models['Model-1'].materials['Material-1'].Elastic(table=((0.1, 0.3), ))
mdb.models['Model-1'].Material(name='Material-2')
mdb.models['Model-1'].materials['Material-2'].Elastic(table=((0.05, 0.3), ))
mdb.models['Model-1'].HomogeneousSolidSection(name='Section-1', 
    material='Material-1', thickness=None)
mdb.models['Model-1'].HomogeneousSolidSection(name='Section-2', 
    material='Material-2', thickness=None)
p = mdb.models['Model-1'].parts['Part-1']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
p = mdb.models['Model-1'].parts['Part-1']
f = p.faces
faces = f.getSequenceFromMask(mask=('[#1 ]', ), )
region = p.Set(faces=faces, name='Set-1')
p = mdb.models['Model-1'].parts['Part-1']
p.SectionAssignment(region=region, sectionName='Section-1', offset=0.0, 
    offsetType=MIDDLE_SURFACE, offsetField='', 
    thicknessAssignment=FROM_SECTION)
p = mdb.models['Model-1'].parts['Part-2']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
p = mdb.models['Model-1'].parts['Part-2']
f = p.faces
faces = f.getSequenceFromMask(mask=('[#1 ]', ), )
region = p.Set(faces=faces, name='Set-2')
p = mdb.models['Model-1'].parts['Part-2']
p.SectionAssignment(region=region, sectionName='Section-2', offset=0.0, 
    offsetType=MIDDLE_SURFACE, offsetField='', 
    thicknessAssignment=FROM_SECTION)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
a.DatumCsysByDefault(CARTESIAN)
p = mdb.models['Model-1'].parts['Part-1']
a.Instance(name='Part-1-1', part=p, dependent=ON)
p = mdb.models['Model-1'].parts['Part-2']
a.Instance(name='Part-2-1', part=p, dependent=ON)
session.viewports['Viewport: 1'].view.setValues(nearPlane=15.9792, 
    farPlane=17.9619, width=12.666, height=6.97253, viewOffsetX=0.734169, 
    viewOffsetY=-0.233989)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(
    adaptiveMeshConstraints=ON)
mdb.models['Model-1'].StaticStep(name='Step-1', previous='Initial')
session.viewports['Viewport: 1'].assemblyDisplay.setValues(step='Step-1')
session.viewports['Viewport: 1'].assemblyDisplay.setValues(
    adaptiveMeshConstraints=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
a.makeIndependent(instances=(a.instances['Part-1-1'], ))
a = mdb.models['Model-1'].rootAssembly
a.makeIndependent(instances=(a.instances['Part-2-1'], ))
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
elemType1 = mesh.ElemType(elemCode=CPS4R, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=CPS3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-1-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
elemType1 = mesh.ElemType(elemCode=CPS4R, elemLibrary=STANDARD, 
    secondOrderAccuracy=OFF, hourglassControl=DEFAULT, 
    distortionControl=DEFAULT)
elemType2 = mesh.ElemType(elemCode=CPS3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-2-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], )
a.seedPartInstance(regions=partInstances, size=0.1, deviationFactor=0.1, 
    minSizeFactor=0.1)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-2-1'], )
a.seedPartInstance(regions=partInstances, size=0.1, deviationFactor=0.1, 
    minSizeFactor=0.1)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], )
a.generateMesh(regions=partInstances)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-2-1'], )
a.generateMesh(regions=partInstances)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
a1 = mdb.models['Model-1'].rootAssembly
a1.InstanceFromBooleanMerge(name='Part-3', instances=(a1.instances['Part-1-1'], 
    a1.instances['Part-2-1'], ), originalInstances=SUPPRESS, 
    mergeNodes=BOUNDARY_ONLY, nodeMergingTolerance=1e-06, domain=BOTH)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
session.viewports['Viewport: 1'].view.setValues(nearPlane=15.8169, 
    farPlane=18.1242, width=14.7366, height=8.11238, viewOffsetX=1.07442, 
    viewOffsetY=-0.252814)
a = mdb.models['Model-1'].rootAssembly
del a.features['Part-3-1']
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
session.viewports['Viewport: 1'].partDisplay.setValues(sectionAssignments=OFF, 
    engineeringFeatures=OFF)
session.viewports['Viewport: 1'].partDisplay.geometryOptions.setValues(
    referenceRepresentation=ON)
p1 = mdb.models['Model-1'].parts['Part-3']
session.viewports['Viewport: 1'].setValues(displayedObject=p1)
del mdb.models['Model-1'].parts['Part-3']
p = mdb.models['Model-1'].parts['Part-1']
session.viewports['Viewport: 1'].setValues(displayedObject=p)
session.viewports['Viewport: 1'].view.setValues(nearPlane=15.9792, 
    farPlane=17.9619, width=12.666, height=6.97253, viewOffsetX=0.85748, 
    viewOffsetY=-0.223972)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
del a.features['Part-1-1']
a = mdb.models['Model-1'].rootAssembly
del a.features['Part-2-1']
a1 = mdb.models['Model-1'].rootAssembly
p = mdb.models['Model-1'].parts['Part-1']
a1.Instance(name='Part-1-1', part=p, dependent=OFF)
p = mdb.models['Model-1'].parts['Part-2']
a1.Instance(name='Part-2-1', part=p, dependent=OFF)
mdb.saveAs(
    pathName='/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae')
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], a.instances['Part-2-1'], )
a.seedPartInstance(regions=partInstances, size=0.1, deviationFactor=0.1, 
    minSizeFactor=0.1)
elemType1 = mesh.ElemType(elemCode=CPS4R, elemLibrary=STANDARD, 
    secondOrderAccuracy=OFF, hourglassControl=DEFAULT, 
    distortionControl=DEFAULT)
elemType2 = mesh.ElemType(elemCode=CPS3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-1-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
f2 = a.instances['Part-2-1'].faces
faces2 = f2.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =((faces1+faces2), )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
elemType1 = mesh.ElemType(elemCode=CPS4R, elemLibrary=STANDARD, 
    secondOrderAccuracy=OFF, hourglassControl=DEFAULT, 
    distortionControl=DEFAULT)
elemType2 = mesh.ElemType(elemCode=CPS3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-1-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
f2 = a.instances['Part-2-1'].faces
faces2 = f2.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =((faces1+faces2), )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], a.instances['Part-2-1'], )
a.generateMesh(regions=partInstances)
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
mdb.Job(name='test_block', model='Model-1', description='', type=ANALYSIS, 
    atTime=None, waitMinutes=0, waitHours=0, queue=None, memory=90, 
    memoryUnits=PERCENTAGE, getMemoryFromAnalysis=True, 
    explicitPrecision=SINGLE, nodalOutputPrecision=SINGLE, echoPrint=OFF, 
    modelPrint=OFF, contactPrint=OFF, historyPrint=OFF, userSubroutine='', 
    scratch='', multiprocessingMode=DEFAULT, numCpus=1, numGPUs=0)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
e1 = a.instances['Part-1-1'].edges
edges1 = e1.getSequenceFromMask(mask=('[#4 ]', ), )
a.Set(edges=edges1, name='Set-1')
#: The set 'Set-1' has been created (1 edge).
a = mdb.models['Model-1'].rootAssembly
e1 = a.instances['Part-1-1'].edges
edges1 = e1.getSequenceFromMask(mask=('[#10 ]', ), )
a.Set(edges=edges1, name='Set-2')
#: The set 'Set-2' has been created (1 edge).
a = mdb.models['Model-1'].rootAssembly
e1 = a.instances['Part-1-1'].edges
edges1 = e1.getSequenceFromMask(mask=('[#8 ]', ), )
a.Set(edges=edges1, name='Set-3')
#: The set 'Set-3' has been created (1 edge).
a = mdb.models['Model-1'].rootAssembly
e1 = a.instances['Part-1-1'].edges
edges1 = e1.getSequenceFromMask(mask=('[#2 ]', ), )
a.Set(edges=edges1, name='Set-4')
#: The set 'Set-4' has been created (1 edge).
mdb.models['Model-1'].rootAssembly.sets.changeKey(fromName='Set-1', 
    toName='Right')
mdb.models['Model-1'].rootAssembly.sets.changeKey(fromName='Set-2', 
    toName='Left')
mdb.models['Model-1'].rootAssembly.sets.changeKey(fromName='Set-3', 
    toName='Bottom')
mdb.models['Model-1'].rootAssembly.sets.changeKey(fromName='Set-4', 
    toName='Top')
a = mdb.models['Model-1'].rootAssembly
s1 = a.instances['Part-1-1'].edges
side1Edges1 = s1.getSequenceFromMask(mask=('[#4 ]', ), )
a.Surface(side1Edges=side1Edges1, name='Surf-1')
#: The surface 'Surf-1' has been created (1 edge).
a = mdb.models['Model-1'].rootAssembly
s1 = a.instances['Part-1-1'].edges
side1Edges1 = s1.getSequenceFromMask(mask=('[#10 ]', ), )
a.Surface(side1Edges=side1Edges1, name='Left')
#: The surface 'Left' has been created (1 edge).
a = mdb.models['Model-1'].rootAssembly
s1 = a.instances['Part-1-1'].edges
side1Edges1 = s1.getSequenceFromMask(mask=('[#2 ]', ), )
a.Surface(side1Edges=side1Edges1, name='Bottom')
#: The surface 'Bottom' has been created (1 edge).
a = mdb.models['Model-1'].rootAssembly
s1 = a.instances['Part-1-1'].edges
side1Edges1 = s1.getSequenceFromMask(mask=('[#8 ]', ), )
a.Surface(side1Edges=side1Edges1, name='Top')
#: The surface 'Top' has been created (1 edge).
mdb.models['Model-1'].rootAssembly.surfaces.changeKey(fromName='Surf-1', 
    toName='Right')
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
mdb.jobs['test_block'].writeInput(consistencyChecking=OFF)
#: The job input file has been written to "test_block.inp".
mdb.ModelFromInputFile(name='Compound_Mesh_1', 
    inputFileName='/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/Compound_Mesh_1.inp')
#: The model "Compound_Mesh_1" has been created.
#: AbaqusException: Ambiguous keyword definition "end".
#:  This occurred while parsing the input file. Run the input file through the batch pre-processor to check for syntax errors.  
#: The model "Compound_Mesh_1" has been imported from an input file. 
#: Please scroll up to check for error and warning messages.
a = mdb.models['Compound_Mesh_1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(step='Initial')
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
del mdb.models['Compound_Mesh_1']
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
mdb.ModelFromInputFile(name='Compound_Mesh_1', 
    inputFileName='/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/Compound_Mesh_1.inp')
#: The model "Compound_Mesh_1" has been created.
#: AbaqusException: Ambiguous keyword definition "end".
#:  This occurred while parsing the input file. Run the input file through the batch pre-processor to check for syntax errors.  
#: The model "Compound_Mesh_1" has been imported from an input file. 
#: Please scroll up to check for error and warning messages.
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
a = mdb.models['Compound_Mesh_1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
del mdb.models['Model-1']
a = mdb.models['Compound_Mesh_1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
session.viewports['Viewport: 1'].setValues(displayedObject=None)
a = mdb.models['Compound_Mesh_1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
openMdb(
    pathName='/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae')
#: The model database "/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae" has been opened.
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
a = mdb.models['Model-1'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
session.viewports['Viewport: 1'].view.setValues(nearPlane=15.9407, 
    farPlane=18.0004, width=13.1565, height=7.24256, viewOffsetX=0.965049, 
    viewOffsetY=-0.192025)
mdb.jobs['test_block'].writeInput(consistencyChecking=OFF)
#: The job input file has been written to "test_block.inp".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
elemType1 = mesh.ElemType(elemCode=CPE4, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=CPE3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-2-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
elemType1 = mesh.ElemType(elemCode=CPS4, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=CPS3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-1-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], )
a.deleteMesh(regions=partInstances)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-2-1'], )
a.deleteMesh(regions=partInstances)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], )
a.generateMesh(regions=partInstances)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-2-1'], )
a.generateMesh(regions=partInstances)
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
mdb.jobs['test_block'].writeInput(consistencyChecking=OFF)
#: The job input file has been written to "test_block.inp".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=ON)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=ON)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-2-1'], )
a.deleteMesh(regions=partInstances)
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-1-1'], )
a.deleteMesh(regions=partInstances)
elemType1 = mesh.ElemType(elemCode=CPE4, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=CPE3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-2-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
elemType1 = mesh.ElemType(elemCode=CPE4, elemLibrary=STANDARD)
elemType2 = mesh.ElemType(elemCode=CPE3, elemLibrary=STANDARD)
a = mdb.models['Model-1'].rootAssembly
f1 = a.instances['Part-1-1'].faces
faces1 = f1.getSequenceFromMask(mask=('[#1 ]', ), )
pickedRegions =(faces1, )
a.setElementType(regions=pickedRegions, elemTypes=(elemType1, elemType2))
a = mdb.models['Model-1'].rootAssembly
partInstances =(a.instances['Part-2-1'], a.instances['Part-1-1'], )
a.generateMesh(regions=partInstances)
mdb.save()
#: The model database has been saved to "/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test_blocks.cae".
session.viewports['Viewport: 1'].assemblyDisplay.setValues(mesh=OFF)
session.viewports['Viewport: 1'].assemblyDisplay.meshOptions.setValues(
    meshTechnique=OFF)
mdb.jobs['test_block'].writeInput(consistencyChecking=OFF)
#: The job input file has been written to "test_block.inp".
mdb.ModelFromInputFile(name='test2', 
    inputFileName='/new/home1/srinath/github/electro_chemo_mech/test/chemo_mech_coupled/test2.inp')
#: The model "test2" has been created.
#: Warning: Line #12200 has been truncated.
#: Warning: Line #12201 has been truncated.
#: Warning: in keyword *NSET, file "test2.inp", line 12199: One or more data lines contain more than 16 items (counting those after trailing commas).  The extra items are deleted.
#: AbaqusException: Invalid set name "2".
#: Invalid set name "3".
#: Invalid set name "4".
#: Invalid set name "1".
#: Invalid set name "5".
#: Invalid set name "2".
#:  This occurred while parsing the input file. Run the input file through the batch pre-processor to check for syntax errors.  
#: The model "test2" has been imported from an input file. 
#: Please scroll up to check for error and warning messages.
a = mdb.models['test2'].rootAssembly
session.viewports['Viewport: 1'].setValues(displayedObject=a)
