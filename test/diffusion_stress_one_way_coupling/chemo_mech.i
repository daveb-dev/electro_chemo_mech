#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  temperature = conc
  volumetric_locking_correction = true
[]

[Mesh]
  file = cube.e
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]

  [./conc]
    initial_condition = 0.0078
    scaling = 1e10
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
  [./TensorMechanics]
    strain = FINITE
    incremental = true
  [../]

  [./diff]
    type = HeatConduction
    variable = conc
    use_displaced_mesh = false
  [../]
  [./diff_t]
    type = HeatConductionTimeDerivative
    variable = conc
    density_name = density
    use_displaced_mesh = false
  [../]
[]

[BCs]
  [./bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = 1
    value = 0.0
  [../]
  [./bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 1
    value = 0.0
  [../]
  [./bottom_z]
    type = DirichletBC
    variable = disp_z
    boundary = 1
    value = 0.0
  [../]

  # [./bottom_conc]
  #   type = DirichletBC
  #   variable = conc
  #   boundary = 1
  #   value = 0.0
  # [../]
  [./bottom_flux]
    type = NeumannBC
    variable = conc
    boundary = 2
    value = 5.18e-16 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
  [../]

[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 0.1
    poissons_ratio = 0.3
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
    type = HeatConductionMaterial
    specific_heat = 1.0
    thermal_conductivity = 1e-6
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0' #copper in g/(cm^3)
  [../]
[]
[Preconditioning]
  [./SMP]
    type = SMP
  [../]
[]
[Executioner]
  type = Transient
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-6

  l_tol = 1e-3

  l_max_its = 100

  dt = 200.0
  end_time = 3600.0
[]
[Debug]
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
