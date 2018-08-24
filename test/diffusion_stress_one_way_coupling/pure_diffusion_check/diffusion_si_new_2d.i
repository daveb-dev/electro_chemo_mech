#Run with 4 procs
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 25
  ny = 100
  xmin = 0.0
  xmax = 1.0
  ymin = 0.0
  ymax = 0.2
[]

[Variables]
  [./conc]
    initial_condition = 0.0078
    scaling = 1e3
  [../]
[]

[AuxVariables]
  [./flux]
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
[]

[Kernels]
  [./diff]
    type = ChemoDiffusion
    variable = conc
    diffusion_coefficient = mobility
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

[]

[Materials]
  [./heat]
    type = DiffusionMaterial
    mobility = 3.0e-7 # D = 10^-13 m^2/s/ R = 8.314/T=298 K
    gas_constant = 1.0
    temperature = 1.0
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
  solve_type = 'LINEAR'
  # [./TimeStepper]
  #   type = SolutionTimeAdaptiveDT
  #   dt = 10
  #
  # [../]
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-10
  l_tol = 1e-3

  l_max_its = 100

  dt = 200
  end_time = 8000
[]
[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
