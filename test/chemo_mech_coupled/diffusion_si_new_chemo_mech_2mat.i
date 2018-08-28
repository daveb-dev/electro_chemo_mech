#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y '
[]

[Mesh]
  # type = GeneratedMesh
  # dim = 2
  # nx = 25
  # ny = 100
  # xmin = 0.0
  # xmax = 1.0
  # ymin = 0.0
  # ymax = 0.2
  file = 'test2.msh'
[]

[Variables]

  [./disp_x]
    # scaling = 1.0e8
  [../]
  [./disp_y]
    # scaling = 1.0e8
  [../]

  [./conc]
    initial_condition = 1.0
    # scaling = 1e1
  [../]
  [./mu_m]
    # scaling = 1e10
  [../]
[]

[AuxVariables]
  [./flux]
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

[]
[AuxKernels]
  [./flux]
    type = DiffusionFluxAux
    variable = flux
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


[]

[Kernels]
  [./TensorMechanics]
    strain = FINITE
    # incremental = true
    # use_displaced_mesh = true
    volumetric_locking_correction = true
  [../]

  [./diff]
    type = ChemoDiffusion
    variable = conc
    stress_based_chemical_potential = mu_m
    diffusion_coefficient = mobility
    use_displaced_mesh = false
  [../]
  [./diff_t]
    type = ChemoDiffusionTimeDerivative
    variable = conc
    # use_displaced_mesh = false
  [../]

  [./mu_x]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y'
    component = 0
    concentration_eigenstrain_name = eigenstrain
    block = 'inner'
  [../]

  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y'
    component = 1
    concentration_eigenstrain_name = eigenstrain
    block = 'inner'
  [../]
  [./mu_x2]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y'
    component = 0
    block = 'outer'
  [../]

  [./mu_y2]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y'
    component = 1
    block = 'outer'
  [../]

[]

[BCs]
  [./bottom_flux]
    type = NeumannBC
    variable = conc
    boundary = top
    value = 1.524e-3 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
    # This is actual current divided by the density for 0.012 A/m^2
    # Molar density of 7.874e4 mol/m^3
  [../]
  [./right_flux]
    type = NeumannBC
    variable = conc
    boundary = right
    value = 1.524e-3 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
    # This is actual current divided by the density for 0.012 A/m^2
    # Molar density of 7.874e4 mol/m^3
  [../]
  [./left_flux]
    type = NeumannBC
    variable = conc
    boundary = left
    value = 1.524e-3 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
    # This is actual current divided by the density for 0.012 A/m^2
    # Molar density of 7.874e4 mol/m^3
  [../]
  [./top_flux]
    type = NeumannBC
    variable = conc
    boundary = bot
    value = 1.524e-3 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
    # This is actual current divided by the density for 0.012 A/m^2
    # Molar density of 7.874e4 mol/m^3
  [../]
  [./bottom_x]
    type = PresetBC
    variable = disp_x
    boundary = '3'
    value = 0.0
  [../]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = '3'
    value = 0.0
  [../]

[]

[Materials]
  [./heat_inner]
    type = DiffusionMaterial
    mobility = 3.0e-7 # D = 10^-13 m^2/s/ R = 8.314/T=298 K
    gas_constant = 1.0
    temperature = 1.0
    block = 'inner'
  [../]
  [./heat_outer]
    type = DiffusionMaterial
    mobility = 0.3
    gas_constant = 1.0
    temperature = 1.0
    block = 'outer'
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0' #silicon in mol/(m^3)
  [../]

  [./elasticity_tensor_inner]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.19
    poissons_ratio = 0.3
    block = 'inner'
  [../]
  [./elasticity_tensor_outer]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.15
    poissons_ratio = 0.26
    block = 'outer'
  [../]
  [./strain_inner]
    type = ComputeFiniteStrain
    eigenstrain_names = eigenstrain
    block = 'inner'
  [../]
  [./strain_outer]
    type = ComputeFiniteStrain
    block = 'outer'
  [../]
  [./conc_strain]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = 0.05
    eigenstrain_name = eigenstrain
    block = 'inner'
  [../]
  [./stress]
    type = ComputeKirchoffStress
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
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-6
  # nl_abs_tol = 1e-10
  l_tol = 1e-3

  l_max_its = 50
  compute_initial_residual_before_preset_bcs = true
  # petsc_options_iname = '-pc_type'
  # petsc_options_value = 'lu'

  dt = 200
  end_time = 3600
[]
[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
