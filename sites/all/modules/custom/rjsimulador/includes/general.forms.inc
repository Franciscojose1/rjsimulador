<?php
/* ******************************************************************************* */
/* *                      General forms functions                                * */
/* ******************************************************************************* */
function generate_group_array($prefix, $index, &$form_state) {
  // This is the field fieldset.
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Group @index', array('@index' => $index)),
    '#collapsible' => TRUE,
    '#description' => t('Filter group'),
    '#attributes' => array('class' => array('group-analysis'))
  );

  if ($index % 3 == 0) {
    $form += array('#suffix' => '<div class="clearfix"></div>');
  }

  $form[$prefix . '_check_age'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by age.'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_check_age']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_check_age'] : 0,
  );

  $form[$prefix . '_age'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('entradas')),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_age']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_age']['from']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_age']['from'] : "",
    '#size' => 1,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_age']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_age']['to']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_age']['to'] : "",
    '#field_suffix' => t('years'),
    '#size' => 1,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_age]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_driving_experience'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by driving experience.'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_check_driving_experience']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_check_driving_experience'] : 0,
  );

  $form[$prefix . '_driving_experience'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('entradas')),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_driving_experience']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_driving_experience']['from']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_driving_experience']['from'] : "",
    '#size' => 1,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_driving_experience']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_driving_experience']['to']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_driving_experience']['to'] : "",
    '#field_suffix' => t('years'),
    '#size' => 1,
    '#maxlength' => 3,
    '#required' => FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_driving_experience]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_check_average_annual_mileage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Filter by average annual mileage.'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_check_average_annual_mileage']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_check_average_annual_mileage'] : 0,
  );

  $form[$prefix . '_average_annual_mileage'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('entradas')),
    '#states' => array(
      'visible' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form[$prefix . '_average_annual_mileage']['from'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('From'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_average_annual_mileage']['from']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_average_annual_mileage']['from'] : "",
    '#size' => 3,
    '#maxlength' => 6,
    '#required' => FALSE,
    '#states' => array(
      'disabled' => array(
        ':input[name="' . $prefix . '_groups[' . $index . '][' . $prefix . '_check_average_annual_mileage]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form[$prefix . '_average_annual_mileage']['to'] = array(
    '#type' => 'textfield',
    '#field_prefix' => t('to'),
    '#default_value' => isset($form_state['values'][$prefix . '_groups'][$index][$prefix . '_average_annual_mileage']['to']) ?
      $form_state['values'][$prefix . '_groups'][$index][$prefix . '_average_annual_mileage']['to'] : "",
    '#field_suffix' => t('kms'),
    '#size' => 3,
    '#maxlength' => 6,
    '#required' => FALSE,
    '#states' => array(
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

function initializeDefaultGroups(&$form_state, $prefix) {
  $grupoDefault = Grupos::getGruposDefault();
  $grupoDefaultKey = variable_get('rjsimulador_grupo_default', 0);

  switch ($grupoDefaultKey) {
    case 1:
      foreach ($grupoDefault as $indice => $arrayDatos) {
        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_age'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_age'] = 0;
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_driving_experience'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_driving_experience'] = 1;

          if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_driving_experience']['from'])) {
            $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_driving_experience']['from'] = $arrayDatos[FilterByInterval::DESDE];
          }

          if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_driving_experience']['to'])) {
            $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_driving_experience']['to'] = $arrayDatos[FilterByInterval::HASTA];
          }
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_average_annual_mileage'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_average_annual_mileage'] = 0;
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_usual_videogame_player'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_usual_videogame_player'] = 0;
        }
      }
      break;
    case 2:
      foreach ($grupoDefault as $indice => $arrayDatos) {
        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_age'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_age'] = 0;
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_driving_experience'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_driving_experience'] = 0;
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_average_annual_mileage'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_average_annual_mileage'] = 1;

          if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_average_annual_mileage']['from'])) {
            $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_average_annual_mileage']['from'] = $arrayDatos[FilterByInterval::DESDE];
          }

          if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_average_annual_mileage']['to'])) {
            $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_average_annual_mileage']['to'] = $arrayDatos[FilterByInterval::HASTA];
          }
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_usual_videogame_player'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_usual_videogame_player'] = 0;
        }
      }
      break;
    default:
      foreach ($grupoDefault as $indice => $arrayDatos) {
        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_age'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_age'] = 1;

          if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_age']['from'])) {
            $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_age']['from'] = $arrayDatos[FilterByInterval::DESDE];
          }

          if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_age']['to'])) {
            $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_age']['to'] = $arrayDatos[FilterByInterval::HASTA];
          }
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_driving_experience'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_driving_experience'] = 0;
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_average_annual_mileage'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_average_annual_mileage'] = 0;
        }

        if (!isset($form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_usual_videogame_player'])) {
          $form_state['values'][$prefix . '_groups'][$indice][$prefix . '_check_usual_videogame_player'] = 0;
        }
      }
      break;
  }
}