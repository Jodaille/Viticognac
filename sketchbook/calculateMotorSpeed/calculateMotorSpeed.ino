#include <Encoder.h>

int motorPin1 = 8;
int motorPin2 = 9;
int enablePin = 5;

#define MOTOR_PIN 9 // Pin de contrôle du moteur sur le L298N
#define ENCODER_A_PIN 2 // Broche de signal A de l'encodeur
#define ENCODER_B_PIN 3 // Broche de signal B de l'encodeur

Encoder encoder(ENCODER_A_PIN, ENCODER_B_PIN);
int desired_rpm = 25; // Vitesse désirée en RPM
double motor_speed; // Vitesse du moteur en RPM
double previous_error = 0;
double integral = 0;
double Kp = 0.1; // Coefficient proportionnel
double Ki = 0.01; // Coefficient intégral
double Kd = 0.001; // Coefficient dérivé

void setup() {
    // Configuration des ports en mode "sortie"
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);

    // Initialisation du port série
    Serial.begin(115200);


}

void loop() {
    // Lire la vitesse actuelle du moteur en RPM
    motor_speed = calculateMotorSpeed();

    // Calculer l'erreur de vitesse
    double error = desired_rpm - motor_speed;

    // Calculer les termes PID
    double proportional = error;
    integral += error;
    double derivative = error - previous_error;

    // Calculer la sortie du contrôleur PID
    double output = (Kp * proportional) + (Ki * integral) + (Kd * derivative);

    // Ajuster la sortie du moteur en fonction de la sortie du contrôleur PID
    analogWrite(enablePin, output);

    // Mettre à jour la valeur de l'erreur précédente
    previous_error = error;

    // Délai pour la boucle de contrôle
    delay(100);
}

double calculateMotorSpeed() {
    // Calculer la vitesse en utilisant les impulsions de l'encodeur
    long pulses = encoder.read();
    double revolutions = pulses / 360.0; // 360 impulsions par tour de moteur
    double elapsed_time = 0.1; // Temps écoulé en secondes (à ajuster en fonction de la fréquence de lecture)
    double speed = (revolutions / elapsed_time) * 60.0; // Convertir en RPM
    return speed;
}