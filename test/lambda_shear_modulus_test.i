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
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
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

[Modules]
  [./TensorMechanics]
    [./Master]
      [./all]
        strain = FINITE
        add_variables = true
        use_displaced_mesh = true
        # use_finite_deform_jacobian = true
        volumetric_locking_correction = false
      [../]
    [../]
  [../]
[]

[Materials]
  [./stress]
    type = ComputeNeoHookeanStress
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    bulk_modulus = 10.0
    shear_modulus = 1.0
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
    variable = disp_y
    boundary = bottom
    value = 0
  [../]
  [./left_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0
  [../]
  [./pressure]
    type = Pressure
    boundary = right
    function = rampConstant
    variable = disp_x
    component = 0
    factor = -2.0
    use_displaced_mesh = false
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
  solve_type = PJFNK
  start_time = 0.0
  dt = 0.2
  #num_steps = 10
  end_time = 1.0
  nl_abs_tol = 1.0e-10
  nl_max_its = 50
[]
[Postprocessors]
  [./force]
    type = SideIntegralVariablePostprocessor
    boundary = right
    variable = stress_11
  [../]
  [./displacement_x]
    type = SideAverageValue
    boundary = right
    variable = disp_x
  [../]
  [./displacement_y]
    type = SideAverageValue
    boundary = top
    variable = disp_y
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
