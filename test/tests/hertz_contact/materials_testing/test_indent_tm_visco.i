# Nano-indentation problem with sharp bekovich type indenter internal angle = 140.6
# Substrate is material that deforms
# Indenter is modeled as a quasi-rigid material with much higher youngs_modulus
# Identer geometry modeled with
  # Indenter4.unv (10 elements along indenter surface)
  # indenter5.unv (6 elements along indenter surface)
  # indenter3.unv (25 ele3ments along indenter surface)
# Problem is an axisymmetric geometry with symmetry enforced at r = 0
# This input file is with FiniteStrainHyperElasticViscoPlastic
 # Substrate Material Properties
  # Youngs Modulus = 10 GPa
  # Poissons ratio = 0.3
  # Reference strain rate = 0.05
# Indenter Material Properties (Purely elastic)
  # Youngs Modulus = 1000 GPa
  # Poissons ratio = 0.0


[Problem]
  coord_type = RZ
[]

[Mesh]#Comment
  file = indenter6.unv
[] # Mesh

[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Variables]

  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]

[] # Variables

[Modules/TensorMechanics/Master]
  [./all]
    use_displaced_mesh = true
    strain = FINITE
    generate_output = 'stress_xx stress_yy vonmises_stress hydrostatic_stress'
  [../]
[]



[BCs]

  [./base_y]
    type = PresetVelocity
    variable = disp_y
    boundary = indenttop
    function = '-1.0*t'
  [../]

  [./base_y1]
    type = PresetBC
    variable = disp_y
    boundary = bot
    value = 0.0
  [../]
  # Not sure whether indenter surface has to be constrained along r direction
  # [./base_x1]
  #   type = PresetBC
  #   variable = disp_x
  #   boundary = sindent
  #   value = 0.0
  # [../]

  [./symm_x]
    type = PresetBC
    variable = disp_x
    boundary = 'left lindent'
    value = 0.0
  [../]
  # [./disp_y]
  #   type = FunctionPresetBC
  #   variable = disp_y
  #   boundary = indenttop
  #   function = disp_y
  # [../]

[] # BCs

[Contact]
  [./dummy_name]
    master = sindent
    slave = ssubstrate
    disp_x = disp_x
    disp_y = disp_y
    penalty = 1e-4
  [../]
[]

[Materials]

  [./stiffStuff1]
    type = ComputeIsotropicElasticityTensor
    block = 'substrate'
    youngs_modulus = 5
    poissons_ratio = 0.3
  [../]
  [./stiffStuff2] # Rigid block
    type = ComputeIsotropicElasticityTensor
    block = 'indenter'
    youngs_modulus = 1000
    poissons_ratio = 0.0
  [../]
  [./stress]
    type = ComputeFiniteStrainElasticStress
    block = 'indenter'
  [../]
  [./stress2]
    type = ComputeMultipleInelasticStress
    inelastic_models = 'plas'
    tangent_operator = nonlinear
    perform_finite_strain_rotations = true
    block = 'substrate'
  [../]
  [./plas]
    type = IsotropicHyperViscoStressUpdate
    absolute_tolerance = 1e-8
    relative_tolerance = 1e-08
    hardening_exponent = 1.8
    saturation_resistance = 8.0e-3
    initial_resistance = 2.0e-3
    hardening_modulus = 40.0e-3
    rate_exponent = 0.18
    reference_strain_rate = 0.05
    effective_inelastic_strain_name = effective_plastic_strain
    max_inelastic_increment = 0.02
    # internal_solve_full_iteration_history = true
    # internal_solve_output_on = always
  [../]

[]
#


[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  []
[]

[Executioner]

  type = Transient

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart'
  petsc_options_value = 'hypre    boomeramg      101'


  line_search = 'none'


  nl_abs_tol = 1e-3
  nl_rel_tol = 1e-4

  l_max_its = 200

  start_time = 0.0
  dt = 0.05
  end_time = 2.0
[] # Executioner

[Postprocessors]
  [./maxdisp]
    type = NodalVariableValue
    nodeid = 7 # 40-1 where 40 is the exodus node number of the top-left node
    variable = disp_y
  [../]
[]

[Outputs]
  [./out]
    type = Exodus
    elemental_as_nodal = true
  [../]
[] # Outputs
