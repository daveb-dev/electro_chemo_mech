#Run with 4 procs
# Fully constrained system. Constrained in bottom_x, constrained in y on both sides
# Pressure applied on top
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  type = FileMesh
  file = 'symmetric_sphere.unv'
[]

[Variables]
  [./disp_x]
    # scaling = 1.0e8
  [../]
  [./disp_y]
    # scaling = 1.0e8
  [../]
  [./disp_z]
    # scaling = 1.0e8
  [../]

  [./conc]
    initial_condition = 1.0
    scaling = 1e-1
  [../]
  [./mu_m]
    scaling = 1e-3
  [../]
[]
[Functions]
  [./flux_t]
    type = ParsedFunction
    vars = 'flux period offset'
    vals = '0.0001 7200.0 200.0'
    value = '-flux*(-1)^(floor(2.0*t/period))'
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

  [./flux_z]
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

  [./flux_z]
    type = DiffusionFluxAux
    variable = flux_z
    component = z
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
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 0
    use_displaced_mesh = true
    # volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 1
    use_displaced_mesh = true
    # volumetric_locking_correction = true
    temperature = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./stress_z]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 2
    use_displaced_mesh = true
    # volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_z
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
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = true
    displacements = 'disp_x disp_y disp_z'
    component = 0
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = true
    displacements = 'disp_x disp_y disp_z'
    component = 1
  [../]
  [./mu_z]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = true
    displacements = 'disp_x disp_y disp_z'
    component = 2
  [../]

[]

[BCs]

  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = '2'
    value = 0.0
  [../]
  [./side_x]
    type = PresetBC
    variable = disp_x
    boundary = '4'
    value = 0.0
  [../]

  [./side_z]
    type = PresetBC
    variable = disp_z
    boundary = '3'
    value = 0.0
  [../]

  [./li_current]
    type = FunctionNeumannBC
    variable = conc
    boundary = '1'
    function = flux_t
    use_displaced_mesh = false
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.19
    poissons_ratio = 0.24
  [../]

  [./strain]
    type = ComputeFiniteStrain
    eigenstrain_names = eigenstrain
  [../]
  [./conc_strain]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = -0.07
    eigenstrain_name = eigenstrain
    use_displaced_mesh = false
  [../]


  [./stress]
    type = ComputeKirchoffStress
  [../]

  [./heat]
    type = DiffusionMaterial
    diffusion_coefficient = 5.0e-4
    activity_coefficient = 1.0
    gas_constant = 8.314e-3
    temperature = 298
    use_displaced_mesh = false
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0e-9' #silicon in mol/(m^3)
  [../]


[]
[Preconditioning]
  [./SMP]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type'
    petsc_options_value = 'lu'
  [../]
[]

[Postprocessors]
  [./ave_conc_inner]
    type = AverageNodalVariableValue
    variable = conc
    boundary = '1'
  [../]
  [./ave_interface_stress_rr]
    type = SideAverageValue
    variable = stress_rr
    boundary = '1'
  [../]
  [./ave_interface_stress_tt]
    type = SideAverageValue
    variable = stress_tt
    boundary = '1'
  [../]
  [./ave_interface_stress_rt]
    type = SideAverageValue
    variable = stress_rt
    boundary = '1'
  [../]
[]


[Executioner]
  type = Transient
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-6

  l_tol = 1e-2

  l_max_its = 100

  dt = 50
  end_time = 7200.0
[]
[Debug]
  # show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
  csv = true
  # sync_times = '200 400 600 800 1000 1200 1400 1600 1800 2000 2200 2400 2600 2800 3000 3200 3400 3600 3800 4000 4200 4400 4600 4800 5000 5200 5400 5600 5800 6000 6200 6400 6600 6800 7000 7200'
  # interval = 10
[]
