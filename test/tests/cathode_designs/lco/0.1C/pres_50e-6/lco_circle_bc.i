#Run with 4 procs
# Fully constrained system. Constrained in bottom_x, constrained in y on both sides
# Pressure applied on top
[GlobalParams]
  displacements = 'disp_x disp_y '
[]

[Mesh]
  type = FileMesh
  file = 'single.msh'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
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
    vals = '1e-05 72000.0 200.0'
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
    value = 50e-6
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
    block = 'inner'
  [../]

  [./stress_y]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y'
    component = 1
    use_displaced_mesh = true
    volumetric_locking_correction = false
    concentration =  conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
    block = 'inner'
  [../]

  [./stress_x2]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y'
    component = 0
    use_displaced_mesh = true
    volumetric_locking_correction = false
    concentration = conc
    concentration_eigenstrain_name = eigenstrain_electrolyte
    variable = disp_x
    block = 'outer'
  [../]

  [./stress_y2]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y'
    component = 1
    use_displaced_mesh = true
    volumetric_locking_correction = false
    concentration = conc
    concentration_eigenstrain_name = eigenstrain_electrolyte
    variable = disp_y
    block = 'outer'
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
    use_displaced_mesh = true
    displacements = 'disp_x disp_y'
    component = 0
    block = 'inner'
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = true
    displacements = 'disp_x disp_y'
    component = 1
    block = 'inner'
  [../]
  [./mu_x2]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain_electrolyte
    displacements = 'disp_x disp_y'
    component = 0
    block = 'outer'
    use_displaced_mesh = true
  [../]

  [./mu_y2]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain_electrolyte
    displacements = 'disp_x disp_y'
    component = 1
    block = 'outer'
    use_displaced_mesh = true
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.19
    poissons_ratio = 0.24
    block = 'inner'
  [../]

  [./elasticity_tensor_electrolyte]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.015
    poissons_ratio = 0.24
    block = 'outer'
  [../]

  [./strain]
    type = ComputeFiniteStrain
    eigenstrain_names = eigenstrain
    block = 'inner'
  [../]
  [./strain_electrolyte]
    type = ComputeFiniteStrain
    eigenstrain_names = eigenstrain_electrolyte
    block = 'outer'
  [../]

  [./conc_strain]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = -0.1
    eigenstrain_name = eigenstrain
    # use_displaced_mesh = true
    block = 'inner'
  [../]

  [./conc_strain_electrolyte]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = 0.0
    eigenstrain_name = eigenstrain_electrolyte
    # use_displaced_mesh = true
    block = 'outer'
  [../]

  [./stress]
    type = ComputeKirchoffStress
  [../]

  [./heat]
    type = DiffusionMaterial
    diffusion_coefficient = 5.0e-4
    activity_coefficient = 1.0
    gas_constant = 8.314e9
    temperature = 300
    use_displaced_mesh = false
    block = 'inner'
  [../]
  [./heat_electrolyte]
    type = DiffusionMaterial
    diffusion_coefficient = 1.0
    activity_coefficient = 1.0
    gas_constant = 8.314e9
    temperature = 300
    use_displaced_mesh = false
    block = 'outer'
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '5.0e-14' #silicon in mol/(m^3)
    block = 'inner'
  [../]

  [./density_electrolyte]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '5.0e-14' #silicon in mol/(m^3)
    block = 'outer'
  [../]

[]

[BCs]
  [./CoupledPressure]
    [./tb]
      boundary = 'top'
      pressure = pressure
      displacements = 'disp_x disp_y'
    [../]
  [../]

  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = 'bot'
    value = 0.0
  [../]
  [./side_x]
    type = PresetBC
    variable = disp_x
    boundary = 'left right'
    value = 0.0
  [../]
  [./li_current]
    type = FunctionNeumannBC
    variable = conc
    boundary = 'inner'
    function = flux_t
    use_displaced_mesh = false
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
    boundary = 'inner'
  [../]
  [./ave_stress_11_right]
    type = SideAverageValue
    variable = stress_11
    boundary = 'right'
  [../]
  [./ave_interface_stress_rr]
    type = SideAverageValue
    variable = stress_rr
    boundary = 'inner'
  [../]
  [./ave_interface_stress_tt]
    type = SideAverageValue
    variable = stress_tt
    boundary = 'inner'
  [../]
  [./ave_interface_stress_rt]
    type = SideAverageValue
    variable = stress_rt
    boundary = 'inner'
  [../]
  [./axial_stress]
    type = SideAverageValue
    variable = axial_direction
    boundary = 'bot'
  [../]
[]


[Executioner]
  type = Transient
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-4

  l_tol = 1e-2

  l_max_its = 100

  dt = 100
  end_time = 72000.0
[]
[Debug]
  # show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
  csv = true
  # sync_times = qtimes
  # interval = intervals
[]