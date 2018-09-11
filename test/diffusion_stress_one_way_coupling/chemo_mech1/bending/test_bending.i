#Run with 4 procs
# Fully constrained system. Constrained in bottom_x, constrained in y on both sides
# Pressure applied on top
[GlobalParams]
  displacements = 'disp_x disp_y'
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  xmax = 10.0
  ymax = 1.0
[]
[MeshModifiers]
  [./center]
    type = BoundingBoxNodeSet
    top_right = '0.01 0.01 0.0'
    bottom_left = '-0.01 -0.01 0.0'
    new_boundary = 'center'
  [../]
[]

[Variables]
  [./disp_x]
    scaling = 1.0e-2
  [../]
  [./disp_y]
    scaling = 1.0e-2
  [../]

  [./conc]
    initial_condition = 0.5
    scaling = 1e13
  [../]
  [./mu_m]
    scaling = 1e-10
  [../]
[]
[Functions]
  [./pry]
    type = ParsedFunction
    value = 1.0e8*(y-0.5)
  [../]
[]

[AuxVariables]
  [./pressure]
  [../]

  [./output_pressure]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./axial_direction]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./flux_x]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./flux_y]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./stress_11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_33]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_11]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_22]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./strain_33]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vol_strain]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_rr]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_tt]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_rt]
    order = CONSTANT
    family = MONOMIAL
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
  [./stress_22]
    type = RankTwoAux
    variable = stress_22
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
  [../]
  [./stress_33]
    type = RankTwoAux
    variable = stress_33
    rank_two_tensor = stress
    index_j = 2
    index_i = 2
  [../]
  [./strain_11]
    type = RankTwoAux
    variable = strain_11
    rank_two_tensor = total_strain
    index_j = 0
    index_i = 0
  [../]
  [./strrain_22]
    type = RankTwoAux
    variable = strain_22
    rank_two_tensor = total_strain
    index_j = 1
    index_i = 1
  [../]
  [./strain_33]
    type = RankTwoAux
    variable = strain_33
    rank_two_tensor = total_strain
    index_j = 2
    index_i = 2
  [../]
  [./output_pressure]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    scalar_type = Hydrostatic
    variable = output_pressure
  [../]
[]


[Kernels]
  [./stress_x]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y'
    component = 0
    use_displaced_mesh = false
    volumetric_locking_correction = false
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y'
    component = 1
    use_displaced_mesh = false
    volumetric_locking_correction = false
    variable = disp_y
  [../]

  [./diff]
    type = ChemoDiffusion
    variable = conc
    stress_based_chemical_potential = mu_m
    use_displaced_mesh = false
    diffusion_coefficient = diffusion_coefficient
  [../]
  [./diff_t]
    type = ChemoDiffusionTimeDerivative
    variable = conc
    use_displaced_mesh = false
  [../]
  [./mu_x]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = false
    displacements = 'disp_x disp_y'
    component = 0
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = false
    displacements = 'disp_x disp_y'
    component = 1
  [../]


[]

[BCs]


  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = 'center'
    value = 0.0
  [../]
  [./side_x]
    type = PresetBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]

  [./pre]
    type = Pressure
    component = 0
    variable = disp_x
    use_displaced_mesh = false
    function = pry
    boundary = right
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 190e9
    poissons_ratio = 0.24
  [../]


  [./strain]
    type = ComputeIncrementalSmallStrain
    displacements = 'disp_x disp_y'
    eigenstrain_names = eigenstrain
  [../]

  [./stress]
    type = ComputeKirchoffStress
  [../]
  [./conc_strain]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = -0.2
    eigenstrain_name = eigenstrain
    use_displaced_mesh = false
  [../]

  [./heat]
    type = DiffusionMaterial
    diffusion_coefficient = 5.0e-14
    activity_coefficient = 1.0
    gas_constant = 8.314e3
    temperature = 298
    use_displaced_mesh = false
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '5.0e3' #silicon in mol/(m^3)
  [../]


[]



[Preconditioning]
  active = pref
  [./pref]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
    petsc_options_value = 'lu mumps'
  [../]
  [./basic]
    type = SMP
    full = true
    petsc_options = '-ksp_diagonal_scale -ksp_diagonal_scale_fix'
    petsc_options_iname = '-pc_type -sub_pc_type -sub_pc_factor_shift_type -pc_asm_overlap'
    petsc_options_value = ' asm      lu           NONZERO                   2'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  steady_state_detection = true
  steady_state_tolerance = 1e-7
  # line_search = l2
  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-5

  l_tol = 1e-3

  l_max_its = 100

  dt = 1
  end_time = 5
  # end_time = 7200.0
  compute_initial_residual_before_preset_bcs = true
[]
[Debug]
  # show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
  csv = true
[]
