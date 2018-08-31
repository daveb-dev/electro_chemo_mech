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
  xmax = 1.0
  ymin = 0.0
  ymax = 0.2
  zmin = 0
  zmax = 1.0
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
    scaling = 1e3
  [../]
  [./mu_m]
    # scaling = 1e10
  [../]

[]

[AuxVariables]
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
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 0
    use_displaced_mesh = false
    volumetric_locking_correction = true
    temperature = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 1
    use_displaced_mesh = false
    volumetric_locking_correction = true
    temperature = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./stress_z]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 2
    use_displaced_mesh = false
    volumetric_locking_correction = true
    temperature = conc
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
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y disp_z'
    component = 1
    concentration_eigenstrain_name = eigenstrain
  [../]
  [./mu_s]
    type = Stresschemicalpotential
    variable = mu_m
    chemical_potential = mu_m
    concentration = conc
    displacements = 'disp_x disp_y disp_z'
    component = 2
    concentration_eigenstrain_name = eigenstrain
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

[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 190e9
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
    type = ComputeFiniteStrainElasticStress
  [../]

  [./heat]
      type = DiffusionMaterial
      diffusion_coefficient = 3.0e-19
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '7.874e4' #silicon in mol/(m^3)
  [../]
[]
[Preconditioning]
  [./SMP]
    type = SMP
    # full = true
  [../]
[]
[Executioner]
  type = Transient
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-6
  # nl_abs_tol = 1e-11

  l_tol = 1e-3

  l_max_its = 100

  dt = 200
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
