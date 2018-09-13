#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 24
  xmin = 0.0
  xmax = 0.2
  ymin = 0.0
  ymax = 0.2
  zmax = 0.2
  zmin = 0.2
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
    initial_condition = 0.0078
    scaling = 1e1
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
    diffusivity = diffusion_coefficient
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
  [./stress_x]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y disp_z'
    component = 0
    use_displaced_mesh = false
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y disp_z'
    component = 1
    use_displaced_mesh = false
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./stress_z]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y disp_z'
    component = 2
    use_displaced_mesh = false
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_z
  [../]

  [./diff]
    type = ChemoDiffusion
    variable = conc
    diffusion_coefficient = diffusion_coefficient
    use_displaced_mesh = false
  [../]
  [./diff_t]
    type = ChemoDiffusionTimeDerivative
    variable = conc
    use_displaced_mesh = false
  [../]
[]

[BCs]
  [./bottom_flux]
    type = NeumannBC
    variable = conc
    boundary = top
    value = 1.524e-5 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
    # This is actual current divided by the density for 0.012 A/m^2
    # Molar density of 7.874e4 mol/m^3
  [../]
  [./right_flux]
    type = DiffusionFluxBC
    variable = conc
    boundary = 'right left front back'
  [../]

  [./bottom_x]
    type = PresetBC
    variable = disp_x
    boundary = bottom
    value = 0.0
  [../]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = bottom
    value = 0.0
  [../]

[]

[Materials]
  [./heat]
    type = DiffusionMaterial
    diffusion_coefficient = 3.0e-3 # D = 10^-13 m^2/s/ R = 8.314/T=298 K
    gas_constant = 8.314e-3
    temperature = 298
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0' #silicon in mol/(m^3)
  [../]

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
    stress_free_concentration = 0.0078
    partial_molar_volume = 0.7
    eigenstrain_name = eigenstrain
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
  nl_abs_tol = 1e-9
  l_tol = 1e-3

  l_max_its = 100
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  dt = 50
  end_time = 1000
[]
[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
