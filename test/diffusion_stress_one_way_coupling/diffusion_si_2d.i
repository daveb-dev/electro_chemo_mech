#Run with 4 procs
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 25
  xmin = 0.0
  xmax = 20.0e-5
  ymin = 0.0
  ymax = 5.0e-5
[]

[Variables]
  [./conc]
    initial_condition = 0.0078
    scaling = 1e15
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
  [./bottom_flux]
    type = NeumannBC
    variable = conc
    boundary = bottom
    value = 5.18e-12 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
  [../]

[]

[Materials]
  [./heat]
    type = HeatConductionMaterial
    specific_heat = 1.0
    thermal_conductivity = 4.0362e-12 # D = 10^-13 m^2/s/ R = 8.314/T=298 K
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0' #silicon in mol/(m^3)
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

  # nl_rel_tol = 1e-20
  # nl_abs_tol = 1e-15

  # l_tol = 1e-3

  l_max_its = 100

  dt = 10.0
  end_time = 3600.0
[]
[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
