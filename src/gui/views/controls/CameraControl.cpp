#include "ui_CameraControl.h"
#include <qvalidator.h>
#include <views/controls/CameraControl.h>

CameraControl::CameraControl() {
}

CameraControl::CameraControl(std::shared_ptr<Camera> camera, QWidget* parent) : QWidget(parent), ui(new Ui::CameraControl) {
    ui->setupUi(this);

    this->camera = camera;
    this->timer = new QTimer(this);
    this->timer->setInterval(16);
    this->timer->start();
    connect(timer, &QTimer::timeout, this, &CameraControl::update);

    auto validator = new QDoubleValidator(this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    validator->setLocale(QLocale::C);

    this->ui->x->setValidator(validator);
    this->ui->y->setValidator(validator);
    this->ui->z->setValidator(validator);
}

CameraControl::~CameraControl() {
}

void CameraControl::update() {
    auto x = QString::number(this->camera->getPosition().x);
    auto y = QString::number(this->camera->getPosition().y);
    auto z = QString::number(this->camera->getPosition().z);
    if (!ui->x->hasFocus()) {
        ui->x->setText(x);
    }
    if (!ui->y->hasFocus()) {
        ui->y->setText(y);
    }
    if (!ui->z->hasFocus()) {
        ui->z->setText(z);
    }
    camera->setPosition(glm::vec3(ui->x->text().toFloat(), ui->y->text().toFloat(), ui->z->text().toFloat()));

    auto [yawValue, pitchValue] = this->camera->getRotation();
    auto yaw = QString::number(yawValue);
    auto pitch = QString::number(pitchValue);
    if (!ui->yaw->hasFocus()) {
        ui->yaw->setText(yaw);
    }
    if (!ui->pitch->hasFocus()) {
        ui->pitch->setText(pitch);
    }
    camera->setRotation(ui->yaw->text().toFloat(), ui->pitch->text().toFloat());

    auto moveSpeed = QString::number(this->camera->getMovementSpeed());
    if (!ui->movement->hasFocus()) {
        ui->movement->setText(moveSpeed);
    }
    camera->setMovementSpeed(ui->movement->text().toFloat());

    auto mouseSensitivity = QString::number(this->camera->getMouseSensitivity());
    if (!ui->rotation->hasFocus()) {
        ui->rotation->setText(mouseSensitivity);
    }
    camera->setMouseSensitivity(ui->rotation->text().toFloat());

    if (!ui->fovSlider->hasFocus()) {
        ui->fovSlider->setValue(this->camera->getFOV());
    }
    ui->fov->setText(QString::number(this->camera->getFOV()));
    camera->setFOV(ui->fovSlider->value());

    if(!ui->acceptInput->hasFocus()) {
        ui->acceptInput->setChecked(this->camera->acceptsInput());
    }
    this->camera->acceptInput(ui->acceptInput->isChecked());
}