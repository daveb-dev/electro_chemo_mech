#Run with 4 procs
[GlobalParams]
  temperature = conc
  volumetric_locking_correction = true
[]

[Mesh]
  file = cube.e
[]

[Variables]
  [./conc]
    initial_condition = 0.1
  [../]
[]


[Kernels]
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

  [./bottom_conc]
    type = DirichletBC
    variable = conc
    boundary = 1
    value = 0.0
  [../]

  # [./top_flux]
  #   type = NeumannBC
  #   variable = conc
  #   boundary = 3
  #   value = 0.1
  # [../]
[]

[Materials]
  [./heat]
    type = HeatConductionMaterial
    specific_heat = 1.0
    thermal_conductivity = 1.0e-15
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

  nl_rel_tol = 1e-14
  l_tol = 1e-3

  l_max_its = 100

  num_steps = 10
  dt = 0.1
  end_time = 1.0
[]

[Outputs]
  exodus = true
[]
