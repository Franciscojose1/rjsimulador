<?php
/**
 * Created by PhpStorm.
 * User: Solus
 * Date: 11/09/2015
 * Time: 21:20
 */

function rj_simulador_services_resources() {
    $resources = array(
        'rj_simulador' => array(
            'operations' => array(
                'retrieve' => array(
                    'help' => 'Recupera una encuestas con sus preguntas',
                    'callback' => '_rj_simulador_retrieve',
                    'access callback' => 'user_access',
                    'access arguments' => array('gestionar estadisticas'),
                    'access arguments append' => FALSE,
                    'args' => array(
                        array(
                            'name' => 'id_encuesta',
                            'type' => 'int',
                            'description' => 'ID de la encuesta a recuperar',
                            'source' => array('path' => '0'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
                'create' => array(
                    'help' => 'Recupera una encuestas con sus preguntas',
                    'callback' => '_rj_simulador_add',
                    'access callback' => 'user_access',
                    'access arguments' => array('gestionar estadisticas'),
                    'access arguments append' => FALSE,
                    'args' => array(
                        array(
                            'name' => 'id_encuesta',
                            'type' => 'int',
                            'description' => 'ID de la encuesta a recuperar',
                            'source' => array('path' => '0'),
                            'optional' => FALSE,
                        ),
                    ),
                ),
            ),
        ),
    );
    return $resources;
}

function _rj_simulador_retrieve(){

}

function _rj_simulador_add(){

}