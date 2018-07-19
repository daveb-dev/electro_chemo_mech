[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 1
  xmin = 0.0
  xmax =  1.0
  ymin = 0.0
  ymax = 1.0
[]

[Variables]
  [./x_disp]
    order = FIRST
    family =  LAGRANGE
  [../]
  [./y_disp]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Functions]
  [./rampConstant]
    type = PiecewiseLinear
    x = '0. 1.'
    y = '0. 1.'
  [../]
  [./constRamp]
    type = ParsedFunction
    value = '-2.0*t'
  [../]

[]

[AuxVariables]
  [./stress_11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_11]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./testTensor_x]
    type = StressDivergenceConcentrationTensor
    variable = x_disp
    component = 0
    displacements = 'x_disp y_disp'
    block = 0
    use_displaced_mesh = false
    use_geometric_jacobian = true
  [../]
  [./testTensor_y]
    type = StressDivergenceConcentrationTensor
    variable = y_disp
    component = 1
    displacements = 'x_disp y_disp'
    block = 0
    use_displaced_mesh = false
    use_geometric_jacobian = true
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    bulk_modulus = 10.0
    shear_modulus = 1.0
    block = 0
  [../]
  [./strain]
    type = Compute2DTlFiniteStrain
    displacements = 'x_disp y_disp'
    out_of_plane_direction = z
    block = 0
  [../]
  [./stress]
    type = ComputeTlNeoHookeanStress
    block = 0
  [../]
[]

[AuxKernels]
  [./stress_11]
    type = RankTwoAux
    variable = stress_11
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
  [../]
[]

[BCs]
  [./bot_y]
    type = PresetBC
    variable = y_disp
    boundary = bottom
    value = 0
  [../]
  [./left_x]
    type = PresetBC
    variable = x_disp
    boundary = left
    value = 0
  [../]
  [./pressure]
    type = Pressure
    boundary = right
    function = rampConstant
    variable = x_disp
    component = 0
    factor = -2.0
    use_displaced_mesh = true
  [../]
[]

[Preconditioning]
  [./SMP]
    type = FDP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  start_time = 0.0
  dt = 0.2
  num_steps = 5
  end_time = 1.0
  nl_abs_tol = 1.0e-6
  nl_max_its = 50
  l_tol = 1e-06
[]
[Postprocessors]
  [./force]
    type = SideIntegralVariablePostprocessor
    boundary = right
    variable = stress_11
    use_displaced_mesh = false
  [../]
  [./displacement_x]
    type = SideAverageValue
    boundary = right
    variable = x_disp
  [../]
  [./displacement_y]
    type = SideAverageValue
    boundary = top
    variable = y_disp
  [../]
[]
[Outputs]
  exodus = true
  console = true
  gnuplot = true
  perf_graph = true
  [./displaced]
    type = Exodus
    file_base = displace
    use_displaced = true
  [../]
[]
