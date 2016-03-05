<?php
/* *************************************************************************** */
/* *                     FORMULARIO POR SIMULACIÓN                             */
/* *************************************************************************** */
function rjsimulador_admin_form_data_analysis($form, &$form_state) {
  // Gestor de simulaciones
  try {
    $gestorSimulaciones = new GestorSimulaciones();
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<p>' . $le->getMessage() . '</p>');
    return $renderArray;
  }

  // Creamos el formulario como tabs verticales
  $form['data_analysis'] = array(
    '#type' => 'vertical_tabs'
  );

  // Cada fieldset es referido al grupo de tabs será una tab vertical
  $form['data_general_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('General data analysis'),
    '#collapsible' => TRUE,
    '#description' => t('General data analysis for all users.'),
    '#group' => 'data_analysis',
    '#attributes' => array('class' => array('rjsimulador-analysis'))
  );

  $form['data_general_analysis'] += generate_form_data_general_analysis($form_state, $gestorSimulaciones);

  $form['data_group_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data analysis by group'),
    '#collapsible' => TRUE,
    '#description' => t('Data analysis by group.'),
    '#group' => 'data_analysis',
    '#attributes' => array('class' => array('rjsimulador-analysis'))
  );

  $form['data_group_analysis'] += generate_form_data_group_analysis($form_state, $gestorSimulaciones);

  $form['data_user_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data analysis by user'),
    '#collapsible' => TRUE,
    '#description' => t('Data analysis by user.'),
    '#group' => 'data_analysis',
    '#attributes' => array('class' => array('rjsimulador-analysis'))
  );

  $form['data_user_analysis'] += generate_form_data_user_analysis($form_state, $gestorSimulaciones);

  return $form;
}

function rjsimulador_admin_form_infractions_analysis($form, &$form_state) {
  // Gestor de simulaciones
  try {
    $gestorSimulaciones = new GestorSimulaciones();
  } catch (LogicException $le) {
    $renderArray = array('#markup' => '<p>' . $le->getMessage() . '</p>');
    return $renderArray;
  }

  // Creamos el formulario como tabs verticales
  $form['data_analysis'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['infractions_general_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('General infraction analysis'),
    '#collapsible' => TRUE,
    '#description' => t('General infractions analysis for all users.'),
    '#group' => 'data_analysis',
    '#attributes' => array('class' => array('rjsimulador-analysis'))
  );

  $form['infractions_general_analysis'] += generate_form_infractions_general_analysis($form_state, $gestorSimulaciones);

  $form['infractions_group_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Infraction analysis by group'),
    '#collapsible' => TRUE,
    '#description' => t('Infraction analysis by group.'),
    '#group' => 'data_analysis',
    '#attributes' => array('class' => array('rjsimulador-analysis'))
  );

  $form['infractions_group_analysis'] += generate_form_infractions_group_analysis($form_state, $gestorSimulaciones);

  $form['infractions_user_analysis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Infraction analysis by user'),
    '#collapsible' => TRUE,
    '#description' => t('Infraction analysis by user.'),
    '#group' => 'data_analysis',
    '#attributes' => array('class' => array('rjsimulador-analysis'))
  );

  $form['infractions_user_analysis'] += generate_form_infractions_user_analysis($form_state, $gestorSimulaciones);

  return $form;
}

/* ******************************************************************************* */
/* *                      Análisis de datos generales                            * */
/* ******************************************************************************* */
function generate_form_data_general_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recuperamos variables locales
  $tiposSimulacion = Constants::getTiposSimulacion();
  $firstKeyTiposSimulacion = key($tiposSimulacion);

  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['data_general_select_simulation']) ?
    $form_state['values']['data_general_select_simulation'] : $firstKeyTiposSimulacion;
  // Recuperamos el tipo de dato que vamos a calcular
  $tipoDatoCalcular = isset($form_state['values']['data_general_select_type']) ?
    $form_state['values']['data_general_select_type'] : CalculateAverageData::CONSUMO_MEDIO;

  // Generamos el título del gráfico
  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of all users<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }
  elseif ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
    $tituloGrafico = t("Average Partida time length of all users<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }
  elseif ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
    $tituloGrafico = t("Average speed by Partida of all users<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }
  else {
    $tituloGrafico = t("Average RPMs by Partida of all users<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }

  // Etiqueta del Eje X
  $labelAxisX[] = t("Users");

  // Generamos el gráfico
  $form['data_general_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#prefix' => '<div id="div-data-general-chart">',
    '#suffix' => '</div>',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  // Recuperamos el cálculo del dato para todos los usuarios que tienen partidas en el simulador
  foreach ($gestorSimulaciones->getListaTodosUsuarios() as $usuario) {
    // Datos medios por simulación
    $dataUsuario = round($usuario->retrieveAllPartidasByIdSimulacion($idSimulacion)
      ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

    $form['data_general_chart']['data_general_user_' . $usuario->getUid()] =
      array(
        '#type' => 'chart_data',
        '#title' => t('User @userName (UID:@userUID)', array(
          '@userName' => $usuario->getName(),
          '@userUID' => $usuario->getUid()
        )),
        '#data' => array($dataUsuario),
      );
  }

  /* ACCIONES DEL FORMULARIO */
  $form['data_general_actions'] = array(
    '#type' => 'actions',
  );

  $form['data_general_actions']['data_general_select_simulation'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $tiposSimulacion,
    '#default_value' => $firstKeyTiposSimulacion,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['data_general_actions']['data_general_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' =>
      array(
        CalculateAverageData::CONSUMO_MEDIO => t('Average consumption'),
        CalculateAverageData::TIEMPO_TOTAL => t('Average time'),
        CalculateAverageData::VELOCIDAD => t('Average speed'),
        CalculateAverageData::RPM => t('Average RPMs')
      ),
    '#default_value' => CalculateAverageData::CONSUMO_MEDIO,
    '#description' => t('Select the data to show.'),
  );

  $form['data_general_actions']['data_general_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter graph'),
    '#name' => 'data_general_filter_button',
    '#prefix' => '<div class="clearfix"></div>',
    '#attributes' => array('class' => array('filter-btn')),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_general_analysis_callback',
      'wrapper' => 'div-data-general-chart',
    ),
  );

  return $form;
}

function ajax_generate_form_data_general_analysis_callback($form, &$form_state) {
  return $form['data_general_analysis']['data_general_chart'];
}

/* ******************************************************************************* */
/* *                   Análisis de datos por grupos                              * */
/* ******************************************************************************* */
function generate_form_data_group_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recuperamos variables locales
  $tiposSimulacion = Constants::getTiposSimulacion();
  $firstKeyTiposSimulacion = key($tiposSimulacion);

  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['data_group_select_simulation']) ?
    $form_state['values']['data_group_select_simulation'] : $firstKeyTiposSimulacion;
  // Recuperamos el dato a calcular
  $tipoDatoCalcular = isset($form_state['values']['data_group_select_type']) ?
    $form_state['values']['data_group_select_type'] : CalculateAverageData::CONSUMO_MEDIO;

  // Generamos el título del gráfico
  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of users groups<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }
  elseif ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
    $tituloGrafico = t("Average Partida time length of users groups<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }
  elseif ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
    $tituloGrafico = t("Average speed by Partida of users groups<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }
  else {
    $tituloGrafico = t("Average RPMs by Partida of users groups<br>@simulacion",
      array('@simulacion' => Constants::getNombreSimulacion($idSimulacion)));
  }

  // Etiquet Eje X
  $labelAxisX[] = t("Groups of users");

  $form['data_group_container'] = array(
    '#prefix' => '<div id="div-data-group">',
    '#suffix' => '</div>',
  );

  $form['data_group_container']['data_group_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#weight' => 0,
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  $form['data_group_container']['data_group_actions'] = array(
    '#type' => 'actions',
    '#weight' => 25,
  );

  $form['data_group_container']['data_group_actions']['data_group_select_simulation'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $tiposSimulacion,
    '#default_value' => $firstKeyTiposSimulacion,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['data_group_container']['data_group_actions']['data_group_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' =>
      array(
        CalculateAverageData::CONSUMO_MEDIO => t('Average consumption'),
        CalculateAverageData::TIEMPO_TOTAL => t('Average time'),
        CalculateAverageData::VELOCIDAD => t('Average speed'),
        CalculateAverageData::RPM => t('Average RPMs')
      ),
    '#default_value' => CalculateAverageData::CONSUMO_MEDIO,
    '#description' => t('Select the data to show.'),
  );

  $form['data_group_container']['data_group_actions']['data_group_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter graph'),
    '#prefix' => '<div class="clearfix"></div>',
    '#attributes' => array('class' => array('filter-btn')),
    '#name' => 'data_group_filter_button',
    '#validate' => array('generate_form_data_group_analysis_validate'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_group_analysis_callback',
      'wrapper' => 'div-data-group',
    ),
  );

  // Número de grupos
  $numeroMinimoGrupos = 1;
  $numeroMaximoGrupos = 6;
  $form_state['data_group_number_groups'] = isset($form_state['data_group_number_groups']) ?
    $form_state['data_group_number_groups'] : $numeroMinimoGrupos;

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'data_group_add_group_button') {
      $form_state['data_group_number_groups']++;
    }
    if ($form_state['clicked_button']['#name'] == 'data_group_delete_group_button') {
      $form_state['data_group_number_groups']--;
    }
  }

  if ($form_state['data_group_number_groups'] < $numeroMaximoGrupos) {
    $form['data_group_container']['data_group_actions']['data_group_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'data_group_add_group_button',
      '#validate' => array('generate_form_data_group_analysis_validate'),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_data_group_analysis_callback',
        'wrapper' => 'div-data-group',
      ),
    );
  }

  if ($form_state['data_group_number_groups'] > $numeroMinimoGrupos) {
    $form['data_group_container']['data_group_actions']['data_group_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'data_group_delete_group_button',
      '#validate' => array('generate_form_data_group_analysis_validate'),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_data_group_analysis_callback',
        'wrapper' => 'div-data-group',
      ),
    );
  }

  $form['data_group_container']['data_group_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Groups'),
    '#collapsible' => FALSE,
    '#description' => t('All groups showed.'),
    '#tree' => TRUE,
    '#weight' => '50',
    '#prefix' => '<div class="clearfix"></div>',
  );

  // Sacamos todos los grupos actuales
  for ($index = 1; $index <= $form_state['data_group_number_groups']; $index++) {
    // Generamos el form de cada grupo
    $form['data_group_container']['data_group_groups'][$index] = generate_group_array('data_group', $index);

    // Listado de todos los usuarios
    $listaUsuarios = $gestorSimulaciones->getListaTodosUsuarios();
    // Solo filtramos si no hay errores
    $titulos = array();
    $formErrors = form_get_errors();
    if (empty($formErrors)) {
      if (isset($form_state['values']['data_group_groups'][$index])) {
        if ($form_state['values']['data_group_groups'][$index]['data_group_check_age'] == 1) {
          $from = $form_state['values']['data_group_groups'][$index]['data_group_age']['from'];
          $to = $form_state['values']['data_group_groups'][$index]['data_group_age']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AGE));

          $titulos[] = t('age from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['data_group_groups'][$index]['data_group_check_driving_experience'] == 1) {
          $from = $form_state['values']['data_group_groups'][$index]['data_group_driving_experience']['from'];
          $to = $form_state['values']['data_group_groups'][$index]['data_group_driving_experience']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::DRIVING_EXPERIENCE));

          $titulos[] = t('driving experience from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['data_group_groups'][$index]['data_group_check_average_annual_mileage'] == 1) {
          $from = $form_state['values']['data_group_groups'][$index]['data_group_average_annual_mileage']['from'];
          $to = $form_state['values']['data_group_groups'][$index]['data_group_average_annual_mileage']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AVERAGE_ANNUAL_MILEAGE));

          $titulos[] = t('average annual mileage from @from to @to kms.', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['data_group_groups'][$index]['data_group_check_usual_videogame_player'] == 1) {
          $radios = $form_state['values']['data_group_groups'][$index]['data_group_usual_videogame_player']['radios'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByEquality(array($radios), FilterByEquality::USUARIO_USUAL_PLAYER));

          $titulos[] = t('@mensaje usual videogame player', array('@mensaje' => $radios == 1 ? t("are") : t("are not")));
        }
      }
    }

    // Recuperamos los datos de la lista de usuarios filtrada
    $retriever = new ListaUsuariosDataRetriever($listaUsuarios);

    // Datos medios por simulación del grupo filtrado
    $dataGroup = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
      ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

    // Generamos el título del grupo según los filtros
    $tituloSustitucion = count($titulos) > 0 ? 'Group @index. Conditions:' : 'Group @index.';
    $arraySustitucion = array('@index' => $index);
    for ($i = 0; $i < count($titulos); $i++) {
      $tituloSustitucion .= '<br>@' . $i;
      $arraySustitucion += array('@' . $i => $titulos[$i]);
    }

    $form['data_group_container']['data_group_chart']['data_group_' . $index] =
      array(
        '#type' => 'chart_data',
        '#title' => t($tituloSustitucion, $arraySustitucion),
        '#data' => array($dataGroup),
      );
  }

  return $form;
}

function generate_form_data_group_analysis_validate($form, &$form_state) {
  $prefix = "data_group";
  validate_group_fields($form_state, $prefix);
}

function ajax_generate_form_data_group_analysis_callback($form, &$form_state) {
  return $form['data_group_analysis']['data_group_container'];
}

/* ******************************************************************************* */
/* *                Tab Análisis de datos por Usuario                            * */
/* ******************************************************************************* */
function generate_form_data_user_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recuperamos variables locales
  $provider = FactoryDataManager::createDataProvider();
  $tiposSimulacion = Constants::getTiposSimulacion();
  $firstKeyTiposSimulacion = key($tiposSimulacion);

  /* RECUPERAMOS LOS DATOS POR AJAX */
  // Recuperamos el Usuario con el que vamos a comparar los datos
  $uidUser = isset($form_state['values']['data_user_select_user']) ?
    $form_state['values']['data_user_select_user'] : key($gestorSimulaciones->getArrayUsuariosUidName());
  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['data_user_select_simulation']) ?
    $form_state['values']['data_user_select_simulation'] : $firstKeyTiposSimulacion;
  // Recuperamos el tipo de dato que vamos a calcular
  $tipoDatoCalcular = isset($form_state['values']['data_user_select_type']) ?
    $form_state['values']['data_user_select_type'] : CalculateAverageData::CONSUMO_MEDIO;

  /* DATOS OBLIGATORIOS PARA EL ANÁLISIS DE DATOS POR USUARIO */
  // Seleccionamos el usuario para el que realizar la comparación
  $usuarioActual = $provider->loadSimulatorUser($uidUser);

  // Genreamos el nombre par el título del gráfico
  if ($tipoDatoCalcular == CalculateAverageData::CONSUMO_MEDIO) {
    $tituloGrafico = t("Average consumption of user @userName<br>@simulacion",
      array(
        '@userName' => $usuarioActual->getName(),
        '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
      ));
  }
  elseif ($tipoDatoCalcular == CalculateAverageData::TIEMPO_TOTAL) {
    $tituloGrafico = t("Average Partida time length of user @userName<br>@simulacion",
      array(
        '@userName' => $usuarioActual->getName(),
        '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
      ));
  }
  elseif ($tipoDatoCalcular == CalculateAverageData::VELOCIDAD) {
    $tituloGrafico = t("Average speed by Partida of user @userName<br>@simulacion",
      array(
        '@userName' => $usuarioActual->getName(),
        '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
      ));
  }
  else {
    $tituloGrafico = t("Average RPMs by Partida of user @userName<br>@simulacion",
      array(
        '@userName' => $usuarioActual->getName(),
        '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
      ));
  }

  // Etiqueta del Eje X
  $labelAxisX[] = t("User and filter groups");

  /* **************************************************************************** */
  /* *              Generamos el formulario y la gráfica                        * */
  /* **************************************************************************** */
  $form['data_user_container'] = array(
    '#prefix' => '<div id="div-data-user">',
    '#suffix' => '</div>',
  );

  $form['data_user_container']['data_user_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#weight' => 0,
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  // Seteamos el usuario actual en el Gestor de Simulaciones
  $gestorSimulaciones->setUsuarioActual($usuarioActual);

  // Recuperamos los datos del usuario actual
  $dataUser = round($usuarioActual->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['data_user_container']['data_user_chart']['data_user_actual'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('User @userName (UID:@userUID)', array(
        '@userName' => $usuarioActual->getName(),
        '@userUID' => $usuarioActual->getUid()
      )),
      '#data' => array($dataUser),
    );

  // Recuperamos los datos del grupo del resto de usuarios
  $listaRestoUsuarios = $gestorSimulaciones->getListaTodosUsuariosExceptoActual();
  // Instanciamos el recuperador de datos
  $retriever = new ListaUsuariosDataRetriever($listaRestoUsuarios);

  $dataRestoUsuarios = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['data_user_container']['data_user_chart']['data_user_rest_of_user'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Rest of users'),
      '#data' => array($dataRestoUsuarios),
    );

  // Recuperamos los datos del grupo del resto de usuarios que está en el mismo grupo de edad que el usuario analizado
  $listaRestoUsuariosMismoGrupoEdad = $listaRestoUsuarios->filterBy(new FilterByInterval($usuarioActual->getGrupoEdad(),
    FilterByInterval::AGE));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoGrupoEdad);
  $dataRestoUsuarioMismoGrupoEdad = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['data_user_container']['data_user_chart']['data_user_same_age_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users which @userGroup', array('@userGroup' => $usuarioActual->getGrupoEdad(TRUE))),
      '#data' => array($dataRestoUsuarioMismoGrupoEdad),
    );

  // Recuperamos los datos del grupo del resto de usuarios que está en el mismo grupo de experiencia conductora que el usuario analizado
  $listaRestoUsuariosMismoGrupoExperiencia = $listaRestoUsuarios->filterBy(new FilterByInterval($usuarioActual->getGrupoExperiencia(), FilterByInterval::DRIVING_EXPERIENCE));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoGrupoExperiencia);
  $dataRestoUsuarioMismoGrupoExperiencia = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['data_user_container']['data_user_chart']['data_user_same_driving_experience_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users which @userGroup', array('@userGroup' => $usuarioActual->getGrupoExperiencia(TRUE))),
      '#data' => array($dataRestoUsuarioMismoGrupoExperiencia),
    );

  // Recuperamos los datos del grupo del resto de usuarios que está en el mismo grupo de kilometraje medio que el usuario analizado
  /*$listaRestoUsuariosMismoGrupoKilometraje = $listaRestoUsuarios->filterBy(new FilterByInterval($usuarioActual->getGrupoKilometrajeMedioAnual(), FilterByInterval::AVERAGE_ANNUAL_MILEAGE));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoGrupoKilometraje);
  $dataRestoUsuarioMismoGrupoKilometraje = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['data_user_container']['data_user_chart']['data_user_same_average_annual_mileage_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users with @userGroup', array('@userGroup' => $usuarioActual->getGrupoKilometrajeMedioAnual(true))),
      '#data' => array($dataRestoUsuarioMismoGrupoKilometraje),
    );*/

  // Recuperamos los datos del grupo del resto de usuarios que está en es del mismo tipo de jugador habitual que el usuario analizado
  $listaRestoUsuariosMismoTipoJugador = $listaRestoUsuarios
    ->filterBy(new FilterByEquality(array($usuarioActual->isUsualVideogamePlayer()), FilterByEquality::USUARIO_USUAL_PLAYER));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoTipoJugador);
  $dataRestoUsuarioMismoTipoJugador = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['data_user_container']['data_user_chart']['data_user_same_average_annual_mileage_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users which @userGroup usual players of games',
        array('@userGroup' => $usuarioActual->isUsualVideogamePlayer(TRUE) ? t("are") : t("are not"))),
      '#data' => array($dataRestoUsuarioMismoTipoJugador),
    );

  /* ACCIONES DEL FORMULARIO */
  $form['data_user_container']['data_user_actions'] = array(
    '#type' => 'actions',
    '#weight' => 25,
  );

  $form['data_user_container']['data_user_actions']['data_user_select_user'] = array(
    '#type' => 'select',
    '#title' => t('Select User'),
    '#options' => $gestorSimulaciones->getArrayUsuariosUidName(),
    '#default_value' => key($gestorSimulaciones->getArrayUsuariosUidName()),
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['data_user_container']['data_user_actions']['data_user_select_simulation'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $tiposSimulacion,
    '#default_value' => $firstKeyTiposSimulacion,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['data_user_container']['data_user_actions']['data_user_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' =>
      array(
        CalculateAverageData::CONSUMO_MEDIO => t('Average consumption'),
        CalculateAverageData::TIEMPO_TOTAL => t('Average time'),
        CalculateAverageData::VELOCIDAD => t('Average speed'),
        CalculateAverageData::RPM => t('Average RPMs')
      ),
    '#default_value' => CalculateAverageData::CONSUMO_MEDIO,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['data_user_container']['data_user_actions']['data_user_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter graph'),
    '#name' => 'data_user_filter_button',
    '#prefix' => '<div class="clearfix"></div>',
    '#attributes' => array('class' => array('filter-btn')),
    '#validate' => array('generate_form_data_user_analysis_validate'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_data_user_analysis_callback',
      'wrapper' => 'div-data-user',
    ),
  );

  // Número de grupos
  $numeroMinimoGrupos = 0;
  $numeroMaximoGrupos = 2;
  $form_state['data_user_number_groups'] = isset($form_state['data_user_number_groups']) ?
    $form_state['data_user_number_groups'] : $numeroMinimoGrupos;

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'data_user_add_group_button') {
      $form_state['data_user_number_groups']++;
    }
    if ($form_state['clicked_button']['#name'] == 'data_user_delete_group_button') {
      $form_state['data_user_number_groups']--;
    }
  }

  if ($form_state['data_user_number_groups'] < $numeroMaximoGrupos) {
    $form['data_user_container']['data_user_actions']['data_user_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'data_user_add_group_button',
      '#validate' => array('generate_form_data_user_analysis_validate'),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_data_user_analysis_callback',
        'wrapper' => 'div-data-user',
      ),
    );
  }

  if ($form_state['data_user_number_groups'] > $numeroMinimoGrupos) {
    $form['data_user_container']['data_user_actions']['data_user_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'data_user_delete_group_button',
      '#validate' => array('generate_form_data_user_analysis_validate'),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_data_user_analysis_callback',
        'wrapper' => 'div-data-user',
      ),
    );
  }

  /* GRUPOS DE COMPARACIÓN A MAYORES */
  $form['data_user_container']['data_user_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Groups'),
    '#collapsible' => TRUE,
    '#collapsed' => $form_state['data_user_number_groups'] < 1,
    '#description' => t('All groups showed.'),
    '#tree' => TRUE,
    '#weight' => '50'
  );

  for ($index = 1; $index <= $form_state['data_user_number_groups']; $index++) {
    $form['data_user_container']['data_user_groups'][$index] = generate_group_array('data_user', $index);
    // Lista de todos los usuarios salvo el actual
    $listaUsuarios = $gestorSimulaciones->getListaTodosUsuariosExceptoActual();
    // Solo filtramos si no hay errores
    $titulos = array();
    if (empty(form_get_errors())) {
      if (isset($form_state['values']['data_user_groups'][$index])) {
        if ($form_state['values']['data_user_groups'][$index]['data_user_check_age'] == 1) {
          $from = $form_state['values']['data_user_groups'][$index]['data_user_age']['from'];
          $to = $form_state['values']['data_user_groups'][$index]['data_user_age']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AGE));

          $titulos[] = t('age from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['data_user_groups'][$index]['data_user_check_driving_experience'] == 1) {
          $from = $form_state['values']['data_user_groups'][$index]['data_user_driving_experience']['from'];
          $to = $form_state['values']['data_user_groups'][$index]['data_user_driving_experience']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::DRIVING_EXPERIENCE));

          $titulos[] = t('driving experience from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['data_user_groups'][$index]['data_user_check_average_annual_mileage'] == 1) {
          $from = $form_state['values']['data_user_groups'][$index]['data_user_average_annual_mileage']['from'];
          $to = $form_state['values']['data_user_groups'][$index]['data_user_average_annual_mileage']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AVERAGE_ANNUAL_MILEAGE));

          $titulos[] = t('average annual mileage from @from to @to kms.', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['data_user_groups'][$index]['data_user_check_usual_videogame_player'] == 1) {
          $radios = $form_state['values']['data_user_groups'][$index]['data_user_usual_videogame_player']['radios'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByEquality(array($radios), FilterByEquality::USUARIO_USUAL_PLAYER));

          $titulos[] = t('@mensaje usual videogame player', array('@mensaje' => $radios == 1 ? t("are") : t("are not")));
        }
      }
    }

    $retriever = new ListaUsuariosDataRetriever($listaUsuarios);

    // Datos medios por simulación
    $dataGroup = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
      ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

    // Generamos el título del grupo según los filtros
    $tituloSustitucion = count($titulos) > 0 ? 'Group @index. Conditions:' : 'Group @index.';
    $arraySustitucion = array('@index' => $index);
    for ($i = 0; $i < count($titulos); $i++) {
      $tituloSustitucion .= '<br>@' . $i;
      $arraySustitucion += array('@' . $i => $titulos[$i]);
    }

    $form['data_user_container']['data_user_chart']['data_user_' . $index] =
      array(
        '#type' => 'chart_data',
        '#title' => t($tituloSustitucion, $arraySustitucion),
        '#data' => array($dataGroup),
      );
  }

  return $form;
}

function generate_form_data_user_analysis_validate($form, &$form_state) {
  $prefix = "data_user";
  validate_group_fields($form_state, $prefix);
}

function ajax_generate_form_data_user_analysis_callback($form, &$form_state) {
  return $form['data_user_analysis']['data_user_container'];
}

/* ******************************************************************************* */
/* *                 Análisis de infracciones generales                          * */
/* ******************************************************************************* */
function generate_form_infractions_general_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recupera las variables locales
  $tiposSimulacion = Constants::getTiposSimulacion();
  $firstKeyTiposSimulacion = key($tiposSimulacion);
  $tiposInfraccion = Constants::getTiposInfracciones();
  $firstKeyTiposInfraccion = key($tiposInfraccion);

  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['infractions_general_select_simulation']) ?
    $form_state['values']['infractions_general_select_simulation'] : $firstKeyTiposSimulacion;
  // Recuperamos el tipo de infracción por el que filtrar
  $idInfraccion = isset($form_state['values']['infractions_general_select_type']) ?
    $form_state['values']['infractions_general_select_type'] : $firstKeyTiposInfraccion;

  // Generamos el título del gráfico
  $tituloGrafico = t("Average number of infraction '@infraccion' by Partida and user<br>@simulacion",
    array(
      '@infraccion' => Constants::getNombreInfraccion($idInfraccion),
      '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
    ));

  // Etiqueta del Eje X
  $labelAxisX[] = t("Users");

  // Generamos el gráfico
  $form['infractions_general_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#prefix' => '<div id="div-infractions-general-chart">',
    '#suffix' => '</div>',
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  // Recuperamos el cálculo del dato para todos los usuarios que tienen partidas en el simulador
  foreach ($gestorSimulaciones->getListaTodosUsuarios() as $usuario) {
    // Media de infracciones por partida, del tipo infracción y la simulación pasadas
    $dataUsuario = round($usuario->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

    $form['infractions_general_chart']['infractions_general_user_' . $usuario->getUid()] =
      array(
        '#type' => 'chart_data',
        '#title' => t('User @userName (UID:@userUID)', array(
          '@userName' => $usuario->getName(),
          '@userUID' => $usuario->getUid()
        )),
        '#data' => array($dataUsuario),
      );
  }

  /* ACCIONES DEL FORMULARIO */
  $form['infractions_general_actions'] = array(
    '#type' => 'actions',
  );

  $form['infractions_general_actions']['infractions_general_select_simulation'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $tiposSimulacion,
    '#default_value' => $firstKeyTiposSimulacion,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['infractions_general_actions']['infractions_general_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select infraction to show'),
    '#options' => $tiposInfraccion,
    '#default_value' => $firstKeyTiposInfraccion,
    '#description' => t('Select the infraction about you want information.'),
  );

  $form['infractions_general_actions']['infractions_general_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter graph'),
    '#name' => 'infractions_general_filter_button',
    '#prefix' => '<div class="clearfix"></div>',
    '#attributes' => array('class' => array('filter-btn')),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_infractions_general_analysis_callback',
      'wrapper' => 'div-infractions-general-chart',
    ),
  );

  return $form;
}

function ajax_generate_form_infractions_general_analysis_callback($form, &$form_state) {
  return $form['infractions_general_analysis']['infractions_general_chart'];
}

/* ******************************************************************************* */
/* *                  Análisis de infracciones por grupos                        * */
/* ******************************************************************************* */
function generate_form_infractions_group_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recupera las variables locales
  $tiposSimulacion = Constants::getTiposSimulacion();
  $firstKeyTiposSimulacion = key($tiposSimulacion);
  $tiposInfraccion = Constants::getTiposInfracciones();
  $firstKeyTiposInfraccion = key($tiposInfraccion);

  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['infractions_group_select_simulation']) ?
    $form_state['values']['infractions_group_select_simulation'] : $firstKeyTiposSimulacion;
  // Recuperamos el tipo de infracción a calcular
  $idInfraccion = isset($form_state['values']['infractions_group_select_type']) ?
    $form_state['values']['infractions_group_select_type'] : $firstKeyTiposInfraccion;

  // Generamos el título del gráfico
  $tituloGrafico = t("Average number of infractions '@infraccion' by Partida and group<br>@simulacion",
    array(
      '@infraccion' => Constants::getNombreInfraccion($idInfraccion),
      '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
    ));

  // Etiquet Eje X
  $labelAxisX[] = t("Groups of users");

  $form['infractions_group_container'] = array(
    '#prefix' => '<div id="div-infractions-group">',
    '#suffix' => '</div>',
  );

  $form['infractions_group_container']['infractions_group_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#weight' => 0,
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  $form['infractions_group_container']['infractions_group_actions'] = array(
    '#type' => 'actions',
    '#weight' => 25,
  );

  $form['infractions_group_container']['infractions_group_actions']['infractions_group_select_simulation'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $tiposSimulacion,
    '#default_value' => $firstKeyTiposSimulacion,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['infractions_group_container']['infractions_group_actions']['infractions_group_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select infraction to show'),
    '#options' => $tiposInfraccion,
    '#default_value' => $firstKeyTiposInfraccion,
    '#description' => t('Select the infraction about you want information.'),
  );

  $form['infractions_group_container']['infractions_group_actions']['infractions_group_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter graph'),
    '#name' => 'infractions_group_filter_button',
    '#prefix' => '<div class="clearfix"></div>',
    '#attributes' => array('class' => array('filter-btn')),
    '#validate' => array('generate_form_infractions_group_analysis_validate'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_infractions_group_analysis_callback',
      'wrapper' => 'div-infractions-group',
    ),
  );

  // Número de grupos
  $numeroMinimoGrupos = 1;
  $numeroMaximoGrupos = 6;
  $form_state['infractions_group_number_groups'] = isset($form_state['infractions_group_number_groups']) ?
    $form_state['infractions_group_number_groups'] : $numeroMinimoGrupos;

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'infractions_group_add_group_button') {
      $form_state['infractions_group_number_groups']++;
    }
    if ($form_state['clicked_button']['#name'] == 'infractions_group_delete_group_button') {
      $form_state['infractions_group_number_groups']--;
    }
  }

  if ($form_state['infractions_group_number_groups'] < $numeroMaximoGrupos) {
    $form['infractions_group_container']['infractions_group_actions']['infractions_group_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'infractions_group_add_group_button',
      '#validate' => array('generate_form_infractions_group_analysis_validate'),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_infractions_group_analysis_callback',
        'wrapper' => 'div-infractions-group',
      ),
    );
  }

  if ($form_state['infractions_group_number_groups'] > $numeroMinimoGrupos) {
    $form['infractions_group_container']['infractions_group_actions']['infractions_group_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'infractions_group_delete_group_button',
      '#validate' => array('generate_form_infractions_group_analysis_validate'),
      '#ajax' => array(
        'callback' => 'ajax_generate_form_infractions_group_analysis_callback',
        'wrapper' => 'div-infractions-group',
      ),
    );
  }

  $form['infractions_group_container']['infractions_group_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Groups'),
    '#collapsible' => FALSE,
    '#description' => t('All groups showed.'),
    '#tree' => TRUE,
    '#weight' => '50'
  );

  // Sacamos todos los grupos actuales
  for ($index = 1; $index <= $form_state['infractions_group_number_groups']; $index++) {
    // Generamos el form de cada grupo
    $form['infractions_group_container']['infractions_group_groups'][$index] = generate_group_array('infractions_group', $index);

    // Listado de todos los usuarios
    $listaUsuarios = $gestorSimulaciones->getListaTodosUsuarios();
    // Solo filtramos si no hay errores
    $titulos = array();
    if (empty(form_get_errors())) {
      if (isset($form_state['values']['infractions_group_groups'][$index])) {
        if ($form_state['values']['infractions_group_groups'][$index]['infractions_group_check_age'] == 1) {
          $from = $form_state['values']['infractions_group_groups'][$index]['infractions_group_age']['from'];
          $to = $form_state['values']['infractions_group_groups'][$index]['infractions_group_age']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AGE));

          $titulos[] = t('age from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['infractions_group_groups'][$index]['infractions_group_check_driving_experience'] == 1) {
          $from = $form_state['values']['infractions_group_groups'][$index]['infractions_group_driving_experience']['from'];
          $to = $form_state['values']['infractions_group_groups'][$index]['infractions_group_driving_experience']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::DRIVING_EXPERIENCE));

          $titulos[] = t('driving experience from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['infractions_group_groups'][$index]['infractions_group_check_average_annual_mileage'] == 1) {
          $from = $form_state['values']['infractions_group_groups'][$index]['infractions_group_average_annual_mileage']['from'];
          $to = $form_state['values']['infractions_group_groups'][$index]['infractions_group_average_annual_mileage']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AVERAGE_ANNUAL_MILEAGE));

          $titulos[] = t('average annual mileage from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['infractions_group_groups'][$index]['infractions_group_check_usual_videogame_player'] == 1) {
          $radios = $form_state['values']['infractions_group_groups'][$index]['infractions_group_usual_videogame_player']['radios'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByEquality(array($radios), FilterByEquality::USUARIO_USUAL_PLAYER));

          $titulos[] = t('@mensaje usual videogame player', array('@mensaje' => $radios == 1 ? t("are") : t("are not")));
        }
      }
    }

    // Recuperamos los datos de la lista de usuarios filtrada
    $retriever = new ListaUsuariosDataRetriever($listaUsuarios);

    // Media del número de infracciones cometidas por simulación del grupo filtrado
    $dataGroup = round($retriever->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

    // Generamos el título del grupo según los filtros
    $tituloSustitucion = count($titulos) > 0 ? 'Group @index. Conditions:' : 'Group @index.';
    $arraySustitucion = array('@index' => $index);
    for ($i = 0; $i < count($titulos); $i++) {
      $tituloSustitucion .= '<br>@' . $i;
      $arraySustitucion += array('@' . $i => $titulos[$i]);
    }

    $form['infractions_group_container']['infractions_group_chart']['infractions_group_' . $index] =
      array(
        '#type' => 'chart_data',
        '#title' => t($tituloSustitucion, $arraySustitucion),
        '#data' => array($dataGroup),
      );
  }

  return $form;
}

function generate_form_infractions_group_analysis_validate($form, &$form_state) {
  $prefix = "infractions_group";
  validate_group_fields($form_state, $prefix);
}

function ajax_generate_form_infractions_group_analysis_callback($form, &$form_state) {
  return $form['infractions_group_analysis']['infractions_group_container'];
}

/* ******************************************************************************* */
/* *                Tab Análisis de infracciones por Usuario                     * */
/* ******************************************************************************* */
function generate_form_infractions_user_analysis(&$form_state, GestorSimulaciones $gestorSimulaciones) {
  // Recupera las variables locales
  $provider = FactoryDataManager::createDataProvider();
  $tiposSimulacion = Constants::getTiposSimulacion();
  $firstKeyTiposSimulacion = key($tiposSimulacion);
  $tiposInfraccion = Constants::getTiposInfracciones();
  $firstKeyTiposInfraccion = key($tiposInfraccion);

  // Recuperamos el Usuario con el que vamos a comparar los datos
  $uidUser = isset($form_state['values']['infractions_user_select_user']) ?
    $form_state['values']['infractions_user_select_user'] : key($gestorSimulaciones->getArrayUsuariosUidName());
  // Recuperamos el idSimulacion por el que filtrar la gráfica
  $idSimulacion = isset($form_state['values']['infractions_user_select_simulation']) ?
    $form_state['values']['infractions_user_select_simulation'] : $firstKeyTiposSimulacion;
  // Recuperamos el dato a calcular
  $idInfraccion = isset($form_state['values']['infractions_user_select_type']) ?
    $form_state['values']['infractions_user_select_type'] : $firstKeyTiposInfraccion;

  /* DATOS OBLIGATORIOS PARA EL ANÁLISIS DE DATOS POR USUARIO */
  // Seleccionamos el usuario para el que realizar la comparación
  $usuarioActual = $provider->loadSimulatorUser($uidUser);

  // Generamos el título del gráfico
  $tituloGrafico = t("Average number of infractions '@infraccion' by Partida from user @userName<br>@simulacion",
    array(
      '@userName' => $usuarioActual->getName(),
      '@infraccion' => Constants::getNombreInfraccion($idInfraccion),
      '@simulacion' => Constants::getNombreSimulacion($idSimulacion)
    ));

  // Etiqueta del Eje X
  $labelAxisX[] = t("User and groups");

  /* **************************************************************************** */
  /* *              Generamos el formulario y la gráfica                        * */
  /* **************************************************************************** */
  $form['infractions_user_container'] = array(
    '#prefix' => '<div id="div-infractions-user">',
    '#suffix' => '</div>',
  );

  $form['infractions_user_container']['infractions_user_chart'] = array(
    '#type' => 'chart',
    '#chart_type' => 'column',
    '#chart_library' => 'highcharts',
    '#title' => $tituloGrafico,
    '#weight' => 0,
    'xaxis' =>
      array(
        '#type' => 'chart_xaxis',
        '#labels' => $labelAxisX,
      ),
  );

  // Seteamos el usuario actual en el Gestor de Simulaciones
  $gestorSimulaciones->setUsuarioActual($usuarioActual);

  // Recuperamos los datos del usuario actual
  $dataUser = round($usuarioActual->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

  $form['infractions_user_container']['infractions_user_chart']['infractions_user_actual'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('User @name', array('@name' => $usuarioActual->getName())),
      '#data' => array($dataUser),
    );

  // Recuperamos los datos del grupo del resto de usuarios
  $listaRestoUsuarios = $gestorSimulaciones->getListaTodosUsuariosExceptoActual();
  // Instanciamos el recuperador de datos
  $retriever = new ListaUsuariosDataRetriever($listaRestoUsuarios);

  $dataRestoUsuarios = round($retriever->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

  $form['infractions_user_container']['infractions_user_chart']['infractions_user_rest_of_user'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Rest of users'),
      '#data' => array($dataRestoUsuarios),
    );

  // Recuperamos los datos del grupo del resto de usuarios que está en el mismo grupo de edad que el usuario analizado
  $listaRestoUsuariosMismoGrupoEdad = $listaRestoUsuarios->filterBy(new FilterByInterval($usuarioActual->getGrupoEdad(),
    FilterByInterval::AGE));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoGrupoEdad);
  $dataRestoUsuarioMismoGrupoEdad = round($retriever->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

  $form['infractions_user_container']['infractions_user_chart']['infractions_user_same_age_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users which @user_group', array('@user_group' => $usuarioActual->getGrupoEdad(true))),
      '#data' => array($dataRestoUsuarioMismoGrupoEdad),
    );

  // Recuperamos los datos del grupo del resto de usuarios que está en el mismo grupo de experiencia conductora que el usuario analizado
  $listaRestoUsuariosMismoGrupoExperiencia = $listaRestoUsuarios->filterBy(new FilterByInterval($usuarioActual->getGrupoExperiencia(), FilterByInterval::DRIVING_EXPERIENCE));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoGrupoExperiencia);
  $dataRestoUsuarioMismoGrupoExperiencia = round($retriever->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

  $form['infractions_user_container']['infractions_user_chart']['infractions_user_same_driving_experience_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users which @user_group', array('@user_group' => $usuarioActual->getGrupoExperiencia(true))),
      '#data' => array($dataRestoUsuarioMismoGrupoExperiencia),
    );

  // Recuperamos los datos del grupo del resto de usuarios que está en el mismo grupo de kilometraje medio que el usuario analizado
  /*$listaRestoUsuariosMismoGrupoKilometraje = $listaRestoUsuarios->filterBy(new FilterByInterval($usuarioActual->getGrupoKilometrajeMedioAnual(), FilterByInterval::AVERAGE_ANNUAL_MILEAGE));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoGrupoKilometraje);
  $dataRestoUsuarioMismoGrupoKilometraje = round($retriever->retrieveAllPartidasByIdSimulacion($idSimulacion)
    ->calculateData(new CalculateAverageData($tipoDatoCalcular)), 2);

  $form['infractions_user_container']['infractions_user_chart']['infractions_user_same_average_annual_mileage_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users with @user_group', array('@user_group' => $usuarioActual->getGrupoKilometrajeMedioAnual(true))),
      '#data' => array($dataRestoUsuarioMismoGrupoKilometraje),
    );*/

  // Recuperamos los datos del grupo del resto de usuarios que está en es del mismo tipo de jugador habitual que el usuario analizado
  $listaRestoUsuariosMismoTipoJugador = $listaRestoUsuarios->filterBy(new FilterByEquality(array($usuarioActual->isUsualVideogamePlayer()),
    FilterByEquality::USUARIO_USUAL_PLAYER));
  $retriever->setListaUsuarios($listaRestoUsuariosMismoTipoJugador);
  $dataRestoUsuarioMismoTipoJugador = round($retriever->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

  $form['infractions_user_container']['infractions_user_chart']['infractions_user_same_average_annual_mileage_group'] =
    array(
      '#type' => 'chart_data',
      '#title' => t('Group of users which @user_group usual players of games',
        array('@user_group' => $usuarioActual->isUsualVideogamePlayer(TRUE) ? t("are") : t("are not"))),
      '#data' => array($dataRestoUsuarioMismoTipoJugador),
    );

  /* ACCIONES DEL FORMULARIO */
  $form['infractions_user_container']['infractions_user_actions'] = array(
    '#type' => 'actions',
    '#weight' => 25,
  );

  $form['infractions_user_container']['infractions_user_actions']['infractions_user_select_user'] = array(
    '#type' => 'select',
    '#title' => t('Select User'),
    '#options' => $gestorSimulaciones->getArrayUsuariosUidName(),
    '#default_value' => key($gestorSimulaciones->getArrayUsuariosUidName()),
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['infractions_user_container']['infractions_user_actions']['infractions_user_select_simulation'] = array(
    '#type' => 'select',
    '#title' => t('Select Simulation'),
    '#options' => $tiposSimulacion,
    '#default_value' => $firstKeyTiposSimulacion,
    '#description' => t('Select the simulation about you want information.'),
  );

  $form['infractions_user_container']['infractions_user_actions']['infractions_user_select_type'] = array(
    '#type' => 'select',
    '#title' => t('Select data to show'),
    '#options' => $tiposInfraccion,
    '#default_value' => $firstKeyTiposInfraccion,
    '#description' => t('Select the infraction about you want information.'),
  );

  $form['infractions_user_container']['infractions_user_actions']['infractions_user_filter_button'] = array(
    '#type' => 'button',
    '#value' => t('Filter graph'),
    '#name' => 'infractions_user_filter_button',
    '#prefix' => '<div class="clearfix"></div>',
    '#attributes' => array('class' => array('filter-btn')),
    '#validate' => array('generate_form_infractions_user_analysis_validate'),
    '#ajax' => array(
      'callback' => 'ajax_generate_form_infractions_user_analysis_callback',
      'wrapper' => 'div-infractions-user',
    ),
  );

  // Número de grupos
  $numeroMinimoGrupos = 0;
  $numeroMaximoGrupos = 2;
  $form_state['infractions_user_number_groups'] = isset($form_state['infractions_user_number_groups']) ?
    $form_state['infractions_user_number_groups'] : $numeroMinimoGrupos;

  if (isset($form_state['clicked_button'])) {
    if ($form_state['clicked_button']['#name'] == 'infractions_user_add_group_button') {
      $form_state['infractions_user_number_groups']++;
    }
    if ($form_state['clicked_button']['#name'] == 'infractions_user_delete_group_button') {
      $form_state['infractions_user_number_groups']--;
    }
  }

  if ($form_state['infractions_user_number_groups'] < $numeroMaximoGrupos) {
    $form['infractions_user_container']['infractions_user_actions']['infractions_user_add_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Add new group'),
      '#name' => 'infractions_user_add_group_button',
      '#ajax' => array(
        'callback' => 'ajax_generate_form_infractions_user_analysis_callback',
        'wrapper' => 'div-infractions-user',
      ),
    );
  }

  if ($form_state['infractions_user_number_groups'] > $numeroMinimoGrupos) {
    $form['infractions_user_container']['infractions_user_actions']['infractions_user_delete_group_button'] = array(
      '#type' => 'button',
      '#value' => t('Delete last group'),
      '#name' => 'infractions_user_delete_group_button',
      '#ajax' => array(
        'callback' => 'ajax_generate_form_infractions_user_analysis_callback',
        'wrapper' => 'div-infractions-user',
      ),
    );
  }

  /* GRUPOS DE COMPARACIÓN A MAYORES */
  $form['infractions_user_container']['infractions_user_groups'] = array(
    '#type' => 'fieldset',
    '#title' => t('Groups'),
    '#collapsible' => TRUE,
    '#collapsed' => $form_state['infractions_user_number_groups'] < 1,
    '#description' => t('All groups'),
    '#tree' => TRUE,
    '#weight' => '50'
  );

  for ($index = 1; $index <= $form_state['infractions_user_number_groups']; $index++) {
    $form['infractions_user_container']['infractions_user_groups'][$index] = generate_group_array('infractions_user', $index);
    // Lista de todos los usuarios excepto el actual
    $listaUsuarios = $gestorSimulaciones->getListaTodosUsuariosExceptoActual();
    // Regeneramos los grupos si no hay errores
    $titulos = array();
    if (empty(form_get_errors())) {
      if (isset($form_state['values']['infractions_user_groups'][$index])) {
        if ($form_state['values']['infractions_user_groups'][$index]['infractions_user_check_age'] == 1) {
          $from = $form_state['values']['infractions_user_groups'][$index]['infractions_user_age']['from'];
          $to = $form_state['values']['infractions_user_groups'][$index]['infractions_user_age']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AGE));

          $titulos[] = t('age from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['infractions_user_groups'][$index]['infractions_user_check_driving_experience'] == 1) {
          $from = $form_state['values']['infractions_user_groups'][$index]['infractions_user_driving_experience']['from'];
          $to = $form_state['values']['infractions_user_groups'][$index]['infractions_user_driving_experience']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::DRIVING_EXPERIENCE));

          $titulos[] = t('driving experience from @from to @to years', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['infractions_user_groups'][$index]['infractions_user_check_average_annual_mileage'] == 1) {
          $from = $form_state['values']['infractions_user_groups'][$index]['infractions_user_average_annual_mileage']['from'];
          $to = $form_state['values']['infractions_user_groups'][$index]['infractions_user_average_annual_mileage']['to'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByInterval(array(
            "desde" => $from,
            "hasta" => $to
          ), FilterByInterval::AVERAGE_ANNUAL_MILEAGE));

          $titulos[] = t('average annual mileage from @from to @to kms.', array(
            '@from' => $from,
            '@to' => $to
          ));
        }

        if ($form_state['values']['infractions_user_groups'][$index]['infractions_user_check_usual_videogame_player'] == 1) {
          $radios = $form_state['values']['infractions_user_groups'][$index]['infractions_user_usual_videogame_player']['radios'];
          $listaUsuarios = $listaUsuarios->filterBy(new FilterByEquality(array($radios), FilterByEquality::USUARIO_USUAL_PLAYER));

          $titulos[] = t('@mensaje usual videogame player', array('@mensaje' => $radios == 1 ? t("are") : t("are not")));
        }
      }
    }

    $retriever = new ListaUsuariosDataRetriever($listaUsuarios);

    // Datos medios por simulación
    $dataGroup = round($retriever->getAverageInfraccionesByPartida($idInfraccion, $idSimulacion), 2);

    // Generamos el título del grupo según los filtros
    $tituloSustitucion = count($titulos) > 0 ? 'Group @index. Conditions:' : 'Group @index.';
    $arraySustitucion = array('@index' => $index);
    for ($i = 0; $i < count($titulos); $i++) {
      $tituloSustitucion .= '<br>@' . $i;
      $arraySustitucion += array('@' . $i => $titulos[$i]);
    }

    $form['infractions_user_container']['infractions_user_chart']['infractions_user_' . $index] =
      array(
        '#type' => 'chart_data',
        '#title' => t($tituloSustitucion, $arraySustitucion),
        '#data' => array($dataGroup),
      );
  }

  return $form;
}

function generate_form_infractions_user_analysis_validate($form, &$form_state) {
  $prefix = "infractions_user";
  validate_group_fields($form_state, $prefix);
}

function ajax_generate_form_infractions_user_analysis_callback($form, &$form_state) {
  return $form['infractions_user_analysis']['infractions_user_container'];
}

/* ******************************************************************************* */
/* *                       General functions                                     * */
/* ******************************************************************************* */
function generate_group_array($prefix, $index) {
  // This is the field fieldset.
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Group @index', array('@index' => $index)),
    '#collapsible' => TRUE,
    '#description' => t('Filter group'),
    '#attributes' => array('class' => array('group-analysis'))
  );

  if ($index%3 == 0) {
    $form += array('#suffix' => '<div class="clearfix"></div>');
  }

  $form[$prefix . '_check_age'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by age.'),
  );

  $form[$prefix . '_age'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('entradas')),
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_age']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From '),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_age']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t(' to '),
    '#field_suffix' => t(' years'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_driving_experience'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by driving experience.'),
  );

  $form[$prefix . '_driving_experience'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('entradas')),
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_driving_experience']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From '),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_driving_experience']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t(' to '),
    '#field_suffix' => t(' years'),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_average_annual_mileage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by average annual mileage.'),
  );

  $form[$prefix . '_average_annual_mileage'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('entradas')),
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_average_annual_mileage']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From '),
    '#size' => 3,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_average_annual_mileage']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t(' to '),
    '#field_suffix' => t(' kms'),
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => FALSE,
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_usual_videogame_player'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by usual videogame player.'),
  );

  $form[$prefix . '_usual_videogame_player'] = array(
    '#type' => 'fieldset',
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_usual_videogame_player]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_usual_videogame_player']['radios'] = array(
    '#type' => 'radios',
    '#title' => t('Usual videogame players?'),
    '#default_value' => 0,
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#states' => array(
      // Only show this field when the 'toggle_me' checkbox is enabled.
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_usual_videogame_player]"]' => array('checked' => FALSE),
      ),
    ),
  );

  return $form;
}

function validate_group_fields(&$form_state, $prefix) {
  foreach ($form_state['values'][$prefix . '_groups'] as $index => $group) {
    $field = "age";
    if ($group[$prefix . '_check_' . $field] == 1) {
      $from = trim($group[$prefix . '_' . $field]['from']);
      $to = trim($group[$prefix . '_' . $field]['to']);
      if ($from == "" || ((int) $from) != $from || $to == "" || ((int) $to) != $to) {
        if ($from == "" || ((int) $from) != $from) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][from',
            t('Age from field must be an integer.'));
        }

        if ($to == "" || ((int) $to) != $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Age to field must be an integer.'));
        }

      }
      else {
        if ($from < 0 || $from > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][from',
            t('Age from field must be between 0 and 120.'));
        }
        if ($to < 0 || $to > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Age to field must be between 0 and 120.'));
        }
        if ($from >= $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field,
            t('Age from field must be smaller than to field.'));
        }
      }
    }

    $field = "driving_experience";
    if ($group[$prefix . '_check_' . $field] == 1) {
      $from = trim($group[$prefix . '_' . $field]['from']);
      $to = trim($group[$prefix . '_' . $field]['to']);
      if ($from == "" || (int) $from != $from || $to == "" || (int) $to != $to) {
        if ($from == "" || (int) $from != $from) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][from',
            t('Driving experience from field must be an integer.'));
        }

        if ($to == "" || (int) $to != $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Driving experience to field must be an integer.'));
        }

      }
      else {
        if ($from < 0 || $from > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][from',
            t('Driving experience from field must be between 0 and 120.'));
        }
        if ($to < 0 || $to > 120) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Driving experience to field must be between 0 and 120.'));
        }
        if ($from >= $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field,
            t('Driving experience from field must be smaller than to field.'));
        }
      }
    }

    $field = "average_annual_mileage";
    if ($group[$prefix . '_check_' . $field] == 1) {
      $from = trim($group[$prefix . '_' . $field]['from']);
      $to = trim($group[$prefix . '_' . $field]['to']);
      if ($from == "" || (int) $from != $from || $to == "" || (int) $to != $to) {

        if ($from == "" || (int) $from != $from) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][from',
            t('Average annual mileage from field must be an integer.'));
        }

        if ($to == "" || (int) $to != $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Average annual mileage to field must be an integer.'));
        }

      }
      else {
        if ($from < 0 || $from > 500000) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][from',
            t('Average annual mileage from field must be between 0 and 120.'));
        }
        if ($to < 0 || $to > 500000) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][to',
            t('Average annual mileage to field must be between 0 and 120.'));
        }
        if ($from >= $to) {
          form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field,
            t('Average annual mileage from field must be smaller than to field.'));
        }
      }
    }

    $field = "usual_videogame_player";
    if ($group[$prefix . '_check_' . $field] == 1) {
      $radios = $group[$prefix . '_' . $field]['radios'];
      if ($radios != 0 && $radios != 1) {
        form_set_error($prefix . '_groups][' . $index . '][' . $prefix . '_' . $field . '][radios',
          t('Must select one radio button.'));
      }
    }
  }
}

