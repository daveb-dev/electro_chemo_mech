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
    # volumetric_locking_correction = true
    generate_output = 'stress_zz strain_zz'
  [../]
[]

[AuxVariables]
  [./peeq]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./fp_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./fp_zz]
    type = RankTwoAux
    variable = fp_zz
    rank_two_tensor = fp
    index_j = 2
    index_i = 2
    execute_on = timestep_end
    block = 0
  [../]
  [./peeq]
    type = MaterialRealAux
    variable = peeq
    property = ep_eqv
    execute_on = timestep_end
    block = 0
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

[UserObjects]
  [./flowstress]
    type = HEVPRambergOsgoodHardening
    yield_stress = 2.8e6
    hardening_exponent = 0.15
    reference_plastic_strain = 0.0004
    intvar_prop_name = ep_eqv
  [../]
  # [./flowstress]
  #   type = HEVAnandHardening
  #   hardening_exponent = 2.6
  #   saturation_hardening = 8e6
  #   hardening_modulus = 40e6
  #   initial_yield_stress = 2e6
  #   intvar_prop_name = ep_eqv
  # [../]
  [./flowrate]
    type = HEVPFlowRatePowerLawJ2
    reference_flow_rate = 0.05
    flow_rate_exponent = 20
    flow_rate_tol = 10
    strength_prop_name = flowstress
  [../]
  [./ep_eqv]
     type = HEVPEqvPlasticStrain
     intvar_rate_prop_name = ep_eqv_rate
  [../]
  [./ep_eqv_rate]
     type = HEVPEqvPlasticStrainRate
     flow_rate_prop_name = flowrate
  [../]
[]

[Materials]

  [./viscop]
    type = FiniteStrainHyperElasticViscoPlastic
    resid_abs_tol = 1e-6
    resid_rel_tol = 1e-8
    maxiters = 50
    max_substep_iteration = 10
    flow_rate_user_objects = 'flowrate'
    strength_user_objects = 'flowstress'
    internal_var_user_objects = 'ep_eqv'
    internal_var_rate_user_objects = 'ep_eqv_rate'
  [../]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 5e9
    poissons_ratio = 0.3
  [../]
[]

[Postprocessors]
  [./stress_zz]
    type = ElementAverageValue
    variable = stress_zz
    block = 'ANY_BLOCK_ID 0'
  [../]
  [./strain_zz]
    type = ElementAverageValue
    variable = strain_zz
    block = 'ANY_BLOCK_ID 0'
  [../]
  [./fp_zz]
    type = ElementAverageValue
    variable = fp_zz
    block = 'ANY_BLOCK_ID 0'
  [../]
  [./peeq]
    type = ElementAverageValue
    variable = peeq
    block = 'ANY_BLOCK_ID 0'
  [../]
  [./u_z]
    type = AverageNodalVariableValue
    variable = uz
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
  dtmax = 10.0
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-6
  dtmin = 1.0e-8
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1.0e-2
    growth_factor = 1.5
    cutback_factor = 0.25
    optimal_iterations = 4
  [../]
  # num_steps = 10
  end_time = 6.0
[]

[Outputs]
  exodus = true
  csv = true
[]
