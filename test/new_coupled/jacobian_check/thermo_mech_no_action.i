#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  temperature = temp
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

  [./temp]
  [../]
[]

[Kernels]
  [./stress_x]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 0
    use_displaced_mesh = false
    volumetric_locking_correction = true
    temperature = temp
    thermal_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 1
    use_displaced_mesh = false
    volumetric_locking_correction = true
    temperature = temp
    thermal_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./stress_z]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y disp_z'
    component = 2
    use_displaced_mesh = false
    volumetric_locking_correction = true
    temperature = temp
    thermal_eigenstrain_name = eigenstrain
    variable = disp_z
  [../]

  [./heat]
    type = HeatConduction
    variable = temp
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

  # [./bottom_temp]
  #   type = DirichletBC
  #   variable = temp
  #   boundary = 1
  #   value = 10.0
  # [../]
  [./top_flux]
    type = NeumannBC
    variable = temp
    boundary = 2
    value = -1.0e-6
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1.0
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ComputeSmallStrain
    eigenstrain_names = eigenstrain
  [../]
  [./thermal_strain]
    type = ComputeThermalExpansionEigenstrain
    stress_free_temperature = 0.0
    thermal_expansion_coeff = 1e-5
    eigenstrain_name = eigenstrain
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]

  [./heat]
    type = HeatConductionMaterial
    specific_heat = 1.0
    thermal_conductivity = 1.0
  [../]

  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0' #silicon in mol/(m^3)
  [../]

  # [./density]
  #   type = Density
  #   density = 1.0
  #   disp_x = disp_x
  #   disp_y = disp_y
  #   disp_z = disp_z
  # [../]
[]
[Preconditioning]
  [./SMP]
    type = FDP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'

  nl_rel_tol = 1e-14
  l_tol = 1e-3

  l_max_its = 100

  dt = 1.0
  end_time = 1.0
[]

[Outputs]
  exodus = true
[]