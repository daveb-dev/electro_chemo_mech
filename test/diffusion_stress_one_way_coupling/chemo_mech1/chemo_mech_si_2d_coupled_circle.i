#Run with 4 procs
[GlobalParams]
  displacements = 'disp_x disp_y '
  # temperature = conc
  volumetric_locking_correction = true
[]

[Mesh]
  # type = GeneratedMesh
  # dim = 2
  # nx = 50
  # ny = 50
  #
  # xmin = 0.0
  # xmax = 6.0
  # ymin = 0.0
  # ymax = 2.0
  # dim = 2
  type = ConcentricCircleMesh
  num_sectors = 36
  radii = '0.5 1.0 1.5'
  rings = '2 5 15'
  inner_mesh_fraction = '0.25'
  has_outer_square = off
  preserve_volumes = off
[]
[MeshModifiers]
  [./center]
    type = BoundingBoxNodeSet
    top_right = '0.01 0.01 0.0'
    bottom_left = '-0.01 -0.01 0.0'
    new_boundary = 'center'
  [../]
[]
[Variables]
  [./disp_x]
    # scaling = 1.0e8
  [../]
  [./disp_y]
    # scaling = 1.0e8
  [../]
  # [./disp_z]
  #   # scaling = 1.0e8
  # [../]

  [./conc]
    initial_condition = 0.0078
    scaling = 1e4
  [../]
  [./mu_m]
    scaling = 1e-7
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
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y'
    component = 0
    use_displaced_mesh = true
    volumetric_locking_correction = true
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_x
  [../]

  [./stress_y]
    type = StressDivergenceTensors
    displacements = 'disp_x disp_y'
    component = 1
    use_displaced_mesh = true
    volumetric_locking_correction = true
    temperature = conc
    concentration_eigenstrain_name = eigenstrain
    variable = disp_y
  [../]

  [./diff]
    type = ChemoDiffusion
    variable = conc
    use_displaced_mesh = false
    diffusion_coefficient = diffusion_coefficient
  [../]
  [./diff_t]
    type = ChemoDiffusionTimeDerivative
    variable = conc
    use_displaced_mesh = false
  [../]
  [./mu_x]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = false
    displacements = 'disp_x disp_y'
    component = 0
  [../]
  [./mu_y]
    type = Stresschemicalpotential
    variable = mu_m
    concentration = conc
    concentration_eigenstrain_name = eigenstrain
    chemical_potential = mu_m
    use_displaced_mesh = false
    displacements = 'disp_x disp_y'
    component = 1
  [../]
[]

[BCs]
  [./bottom_x]
    type = PresetBC
    variable = disp_x
    boundary = center
    value = 0.0
  [../]
  [./bottom_y]
    type = PresetBC
    variable = disp_y
    boundary = center
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
    boundary = outer
    value = 5.18e-5 # 5mA/cm^2 current density or 5.18e-4mol/m^2/s
  [../]

[]

[Materials]
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
    use_displaced_mesh = false
  [../]
  [./stress]
    type = ComputeKirchoffStress
  [../]

  [./heat]
    type = DiffusionMaterial
    diffusion_coefficient = 3.0e-5
    activity_coefficient = 1.0
    gas_constant = 8.314e-3
    temperature = 298
    use_displaced_mesh = false
  [../]
  [./density]
    type = GenericConstantMaterial
    prop_names = 'density'
    prop_values = '1.0e-7' #silicon in mol/(m^3)
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

  nl_rel_tol = 1e-3
  nl_abs_tol = 1e-4

  l_tol = 1e-2

  l_max_its = 100

  dt = 200
  end_time = 7200.0
[]
[Debug]
  # show_material_props = true
  show_var_residual_norms = true
[]
[Outputs]
  exodus = true
  print_linear_residuals = true
[]
