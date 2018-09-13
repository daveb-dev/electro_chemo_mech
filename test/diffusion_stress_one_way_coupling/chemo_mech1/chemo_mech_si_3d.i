#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 5
  ny = 5
  nz = 5
  xmin = 0.0
  xmax = 0.2
  ymin = 0.0
  ymax = 0.2
  zmin = 0
  zmax = 0.2
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
    scaling = 1e5
  [../]
  [./mu_m]
    # scaling = 1e8
  [../]

[]

[AuxVariables]
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
[]

[AuxKernels]
  [./flux_y]
    type = DiffusionFluxAux
    variable = flux_y
    component = y
    diffusivity = diffusion_coefficient
    diffusion_variable = conc
  [../]

  [./flux_x]
    type = DiffusionFluxAux
    variable = flux_x
    component = x
    diffusivity = diffusion_coefficient
    diffusion_variable = conc
  [../]

  [./flux_z]
    type = DiffusionFluxAux
    variable = flux_z
    component = z
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

[]


[Kernels]
  [./stress_x]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y disp_z'
    component = 0
    # use_displaced_mesh = false
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y disp_z'
    component = 1
    # use_displaced_mesh = false
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./stress_z]
    type = StressDivergenceConcentrationTensors
    displacements = 'disp_x disp_y disp_z'
    component = 2
    # use_displaced_mesh = false
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_z
  [../]
  [./diff]
    type = ChemoDiffusion
    variable = conc
    use_displaced_mesh = false
    diffusion_coefficient = diffusion_coefficient
    stress_based_chemical_potential = mu_m
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
    displacements = 'disp_x disp_y disp_z'
    component = 0
    concentration_eigenstrain_name = eigenstrain
    use_displaced_mesh = false
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y disp_z'
    component = 1
    concentration_eigenstrain_name = eigenstrain
    use_displaced_mesh = false
  [../]
  [./mu_s]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y disp_z'
    component = 2
    concentration_eigenstrain_name = eigenstrain
    use_displaced_mesh = false
  [../]

[]

[BCs]
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
  [./bottom_z]
    type = PresetBC
    variable = disp_z
    boundary = bottom
    value = 0.0
  [../]

  # [./bottom_conc]
  #   type = DirichletBC
  #   variable = conc
  #   boundary = 2
  #   value = 0.01
  # [../]
  [./bottom_flux]
    type = NeumannBC
    variable = conc
    boundary = top
    value = 5.18e-5 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
  [../]
  [./side_flux]
    type = DiffusionFluxBC
    variable = conc
    boundary = 'left right front back'
  [../]

[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.1
    poissons_ratio = 0.26
    use_displaced_mesh = true
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

  [./heat]
      type = DiffusionMaterial
      diffusion_coefficient = 3.0e-7 # um^2/s
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '7.874e-4' #silicon in mol/(um^3)
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

  nl_rel_tol = 1e-2
  nl_abs_tol = 1e-5

  l_tol = 1e-3

  l_max_its = 100

  dt = 50
  end_time = 5000.0
[]
[Debug]
  # show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
