#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 1
  ny = 1
  nz = 1
  xmin = 0.0
  xmax = 1
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 1
  elem_type = HEX8
  parallel_type = REPLICATED
[]

[MeshModifiers]
  [./createBotnode]
    type = BoundingBoxNodeSet
    new_boundary = 10
    bottom_left = '-0.1 -0.1 -0.1'
    top_right = '0.1 0.1 0.1'
  [../]
[]
[Functions]
  [./conc_t]
    type = ParsedFunction
    value = '1.0 - 0.1*t'
  [../]
[]

[Variables]

  [./disp_x]
    # scaling = 1.0e8
  [../]
  [./disp_y]
    # scaling = 1.0e8
  [../]
  [./disp_z]
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
  [./current]
    order = FIRST
    family = LAGRANGE
  [../]
  [./voltage]
    order = FIRST
    family = LAGRANGE
  [../]
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
    volumetric_locking_correction = true
  [../]

  [./diff]
    type = ChemoDiffusion
    variable = conc
    stress_based_chemical_potential = mu_m
    diffusion_coefficient = mobility
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

  [./mu_z]
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
  [./const_conc_boundary]
    type = FunctionPresetBC
    variable = conc
    function = conc_t
    boundary = 'top left bottom right'
  [../]
  [./corner_x]
    type = PresetBC
    variable = disp_x
    boundary = 10
    value = 0.0
  [../]
  [./corner_y]
    type = PresetBC
    variable = disp_y
    boundary = 10
    value = 0.0
  [../]
  [./corner_z]
    type = PresetBC
    variable = disp_z
    boundary = 10
    value = 0.0
  [../]

[]

[Materials]
  [./heat_inner]
    type = DiffusionMaterial
    mobility = 5.0e-4 # D = 10^-13 m^2/s/ R = 8.314/T=298 K
    gas_constant = 1.0
    temperature = 1.0
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
  [../]

  [./strain_inner]
    type = ComputeFiniteStrain
    eigenstrain_names = eigenstrain
  [../]
  [./conc_strain]
    type = ComputeConcentrationEigenstrain
    concentration = conc
    stress_free_concentration = 1.0
    partial_molar_volume = -0.05
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
  nl_abs_tol = 1e-3
  l_tol = 1e-3

  l_max_its = 50
  compute_initial_residual_before_preset_bcs = true
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  dt = 0.1
  num_steps = 10
  # end_time = 0.1
[]
[Debug]
  show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
