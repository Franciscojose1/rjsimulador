<?php

function rj_simulador_pagina_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
  );

  // Adds a simple submit button that refreshes the form and clears its contents -- this is the default behavior for forms.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  return $form;
}
