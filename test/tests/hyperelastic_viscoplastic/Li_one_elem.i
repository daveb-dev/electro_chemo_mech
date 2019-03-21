[Mesh]
  type = GeneratedMesh
  xmax = 1.0e-3
  ymax = 1.0e-3
  zmax = 1.0e-3
  dim = 3
  elem_type = HEX8
[]
[GlobalParams]
  displacements = 'ux uy uz'
[]

[Variables]
  [./ux]
  [../]
  [./uy]
  [../]
  [./uz]
  [../]
[]

[Functions]
  [./vel]
    type = PiecewiseLinear
    data_file = 'amp_curve.csv'
    format = columns
    # scale_factor = 1e-2
  [../]
  [./vel2]
    type = ParsedFunction
    value = '0.0003*(exp(0.3002*t))'
  [../]
  [./vel1]
    type = ParsedFunction
    value = '(0.0003/0.3002)*(exp(0.3002*t)-1)'
  [../]
[]

[Modules/TensorMechanics/Master]
  [./all]
    add_variables = true
    strain = FINITE
    use_displaced_mesh = true
    volumetric_locking_correction = true
    generate_output = 'stress_zz strain_zz'
  [../]
[]

[AuxVariables]
  [./Fp_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./plastic_strain]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strength]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./fp_zz]
    type = RankTwoAux
    variable = Fp_zz
    rank_two_tensor = plastic_distortion
    index_i = 2
    index_j = 2
    execute_on = timestep_end
  [../]
  [./peeq]
    type = MaterialRealAux
    variable = plastic_strain
    property = effective_plastic_strain
    execute_on = timestep_end
  [../]
  [./strength]
    type = MaterialRealAux
    variable = strength
    property = yield_strength
    execute_on = 'TIMESTEP_END'
  [../]
[]

[BCs]
  [./symmy]
    type = PresetBC
    variable = uy
    boundary = bottom
    value = 0
  [../]
  [./symmx]
    type = PresetBC
    variable = ux
    boundary = left
    value = 0
  [../]
  [./symmz]
    type = PresetBC
    variable = uz
    boundary = back
    value = 0
  [../]
  [./tdisp]
    type = PresetVelocity
    variable = uz
    boundary = front
    function = vel2
  [../]
[]


[Materials]

  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 5e9
    poissons_ratio = 0.3
  [../]
  [./stress]
    type = ComputeMultipleInelasticStress
    inelastic_models = 'plas'
    tangent_operator = nonlinear
    perform_finite_strain_rotations = true
  [../]
  [./plas]
    type = IsotropicHyperViscoStressUpdate
    hardening_exponent = 1.8
    saturation_resistance = 8.0e6
    initial_resistance = 2.0e6
    hardening_modulus = 40.0e6
    rate_exponent = 0.18
    reference_strain_rate = 0.05
    effective_inelastic_strain_name = effective_plastic_strain
    max_inelastic_increment = 0.0001
  [../]
[]

[Postprocessors]
  [./stress_zz]
    type = ElementAverageValue
    variable = stress_zz
  [../]
  [./strain_zz]
    type = ElementAverageValue
    variable = strain_zz
  [../]
  [./u_z]
    type = AverageNodalVariableValue
    variable = uz
    boundary = front
  [../]
  [./peeq]
    type = ElementAverageValue
    variable = plastic_strain
  [../]
  [./fp_zz]
    type = ElementAverageValue
    variable = Fp_zz
  [../]
  [./stregnth]
    type = ElementAverageValue
    variable = strength
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  #Preconditioned JFNK (default)
  dt = 0.01
  solve_type = 'PJFNK'
  petsc_options_iname = -pc_type
  petsc_options_value = lu
  dtmax = 0.05
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-6
  dtmin = 1.0e-8
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1.0e-2
    growth_factor = 1.5
    cutback_factor = 0.25
    optimal_iterations = 5
  [../]
  # num_steps = 10
  end_time = 6.0
[]

[Outputs]
  exodus = true
  csv = true
[]

[Debug]
  show_material_props = true
[]