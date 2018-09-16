#Run with 4 procs
# Fully constrained system. Constrained in bottom_x, constrained in y on both sides
# Pressure applied on top
[GlobalParams]
  displacements = 'disp_x disp_y '
[]

[Mesh]
  # dim = 2
  # type = GeneratedMesh
  # xmax = 1.0e-6
  # xmin = 0.0
  # ymax = 1.0e-6
  # nx = 10
  # ny = 10
  type = ConcentricCircleMesh
  radii = "0.5 1.0 2.0"
  rings = "2 5 10"
  inner_mesh_fraction = 0.25
  num_sectors = 6
  has_outer_square = off
  preserve_volumes = off
[]
[MeshModifiers]
  # [./center]
  #   type = BoundingBoxNodeSet
  #   top_right = '0.01 0.01 0.0'
  #   bottom_left = '-0.01 -0.01 0.0'
  #   new_boundary = 'center'
  # [../]
  [./all]
    type = BoundingBoxNodeSet
    top_right = '2.1 2.1 0.0'
    bottom_left = '-2.1 -2.1 0.0'
    new_boundary = 'all'
  [../]
[]

[Variables]
  [./disp_x]
    # scaling = 1.0e-5
  [../]
  [./disp_y]
    # scaling = 1.0e-5
  [../]
  # [./disp_z]
  #   # scaling = 1.0e8
  # [../]

  [./conc]
    initial_condition = 1.0
    scaling = 1e1
  [../]
  [./mu_m]
    # initial_condition = 10000.0
     scaling = 1e-3
  [../]
[]
[Functions]
  [./flux_t]
    type = ParsedFunction
    vars = 'flux period offset'
    vals = '0.0001 7200.0 0.0'
    value = '-flux'
  [../]
  [./conc_t]
    type = PiecewiseLinear
    x = '0. 1.'
    y = '0. 1.'
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
  [./pressure]
    type = ConstantAux
    variable = pressure
    value = 0.0
  [../]

  [./flux_x]
    type = DiffusionFluxAux
    variable = flux_x
    component = x
    diffusivity = mobility
    diffusion_variable = conc
  [../]

  [./flux_y]
    type = DiffusionFluxAux
    variable = flux_y
    component = y
    diffusivity = mobility
    diffusion_variable = conc
  [../]

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
  [./vol]
    type = RankTwoScalarAux
    rank_two_tensor = total_strain
    variable = vol_strain
    scalar_type = VolumetricStrain
  [../]
  [./stress_rr]
    type = CylindricalRankTwoAux
    variable = stress_rr
    rank_two_tensor = stress
    index_j = 0
    index_i = 0
    center_point = '0 0 0'
  [../]
  [./stress_tt]
    type = CylindricalRankTwoAux
    variable = stress_tt
    rank_two_tensor = stress
    index_j = 1
    index_i = 1
    center_point = '0 0 0'
  [../]
  [./stress_rt]
    type = CylindricalRankTwoAux
    variable = stress_rt
    rank_two_tensor = stress
    index_j = 0
    index_i = 1
    center_point = '0 0 0'
  [../]
  [./output_pressure]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    scalar_type = Hydrostatic
    variable = output_pressure
  [../]
  [./axial_stress]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    scalar_type = AxialStress
    variable = axial_direction
    direction = '0 1 0'
  [../]
[]


[Kernels]
  [./stress_x]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y'
    component = 0
    use_displaced_mesh = true
    volumetric_locking_correction = false
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y'
    component = 1
    use_displaced_mesh = true
    volumetric_locking_correction = false
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./diff]
    type = ChemoDiffusion
    variable = conc

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
    chemical_potential = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    use_displaced_mesh = true
    displacements = 'disp_x disp_y'
    component = 0
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    use_displaced_mesh = true
    displacements = 'disp_x disp_y'
    component = 1
  [../]
[]

[BCs]
  # [./CoupledPressure]
  #   [./tb]
  #     boundary = 'top'
  #     pressure = pressure
  #     displacements = 'disp_x disp_y'
  #   [../]
  # [../]

  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = outer
    value = 0.0
  [../]
  [./side_x]
    type = PresetBC
    variable = disp_x
    boundary = outer
    value = 0.0
  [../]
  # [./li_conc]
  #   type = FunctionDirichletBC
  #   variable = conc
  #   boundary = all
  #   function = conc_t
  # [../]
  [./li_current]
    type = FunctionNeumannBC
    variable = conc
    boundary = outer
    function = flux_t
    # use_displaced_mesh = false
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.1
    poissons_ratio = 0.26
  [../]


  [./strain]
    type = ComputeFiniteStrain
    eigenstrain_names = eigenstrain
  [../]
  [./conc_strain]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = -0.1
    eigenstrain_name = eigenstrain
    # use_displaced_mesh = true
  [../]

  [./stress]
    type = ComputeKirchoffStress
  [../]

  [./heat]
    type = DiffusionMaterial
    diffusion_coefficient = 1.0e-4
    activity_coefficient = 1.0
    gas_constant = 8.314e9
    temperature = 300
    use_displaced_mesh = false
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = 7.874e-12 #silicon in mol/(m^3)
  [../]

[]


[Postprocessors]
  # [./ave_conc_inner]
  #   type = AverageNodalVariableValue
  #   variable = conc
  #   boundary = outer
  # [../]
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
  [./fdp]
    type = FDP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  # line_search = l2
  nl_rel_tol = 1e-2
  nl_abs_tol = 1e-6

  l_tol = 1e-2

  l_max_its = 100

  dt = 100
  end_time = 2000
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