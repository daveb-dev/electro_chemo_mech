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
    x = '0. 10.'
    y = '0. 10.'
    scale_factor = 2.0
  [../]
[]

[AuxVariables]
  [./stress_11]
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
  [../]
  [./testTensor_y]
    type = StressDivergenceConcentrationTensor
    variable = y_disp
    component = 1
    displacements = 'x_disp y_disp'
  [../]
[]

[Materials]
  [./strain]
    type = ComputeTlFiniteStrainBase
    displacements = 'x_disp y_disp'
  [../]
  [./stress]
    type = ComputeTlNeoHookeanStress
  [../]

  [./elasticity_tensor]
    type = ComputeElasticityTensor
    block = 0
    fill_method = symmetric_isotropic
    C_ijkl = '10.0 1.0'
    displacements = 'x_disp y_disp'
  [../]
[]

[AuxKernels]
  [./stress_11]
    type = RankTwoAux
    variable = stress_11
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
  [../]
[]

[BCs]
  active = 'left_y left_x'
  [./left_y]
    type = PresetBC
    variable = y_disp
    boundary = left
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
  [../]

[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  start_time = 0.0
  dt = 0.2
  num_steps = 5
  end_time = 1.0
  nl_abs_tol = 1.0e-6
  nl_max_its = 50
[]

[Outputs]
  exodus = true
[]
