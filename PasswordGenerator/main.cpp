#include <QApplication>
#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <random>

void onGenerateClick(QSpinBox* spinBox, QCheckBox* numbersCheckBox, QCheckBox* lettersCheckBox,
                     QCheckBox* specialCharactersCheckBox, QLineEdit* generatedPasswordLineEdit);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget* widget = new QWidget();
    widget->setWindowTitle("Password Generator");
    widget->setMinimumSize(400, 250);
    widget->setWindowIcon(QIcon("PasswordGenerator.png"));

    QLabel* lengthPasswordLabel = new QLabel(widget);
    lengthPasswordLabel->setText("Set the password length (8-25):");
    lengthPasswordLabel->setMinimumSize(150, 25);

    QSpinBox* spinBox = new QSpinBox(widget);
    spinBox->setMinimum(8);
    spinBox->setMaximum(25);
    spinBox->setMinimumSize(150, 25);

    QLabel* chooseLabel = new QLabel(widget);
    chooseLabel->setText("Select options:");
    chooseLabel->setMinimumSize(150, 25);

    QCheckBox* numbersCheckBox = new QCheckBox(widget);
    numbersCheckBox->setText("Numbers");
    numbersCheckBox->setMaximumSize(150, 25);

    QCheckBox* lettersCheckBox = new QCheckBox(widget);
    lettersCheckBox->setText("Letters");
    lettersCheckBox->setMinimumSize(150, 25);

    QCheckBox* specialCharactersCheckBox = new QCheckBox(widget);
    specialCharactersCheckBox->setText("Special characters");
    specialCharactersCheckBox->setMinimumSize(150, 25);

    QPushButton* generateButton = new QPushButton("Generate", widget);
    generateButton->setMinimumSize(150, 25);

    QLabel* preGeneratedPasswordLabel = new QLabel(widget);
    preGeneratedPasswordLabel->setText("Your password:");
    preGeneratedPasswordLabel->setMinimumSize(150, 25);

    QLineEdit* generatedPasswordLineEdit = new QLineEdit(widget);
    generatedPasswordLineEdit->setMinimumSize(150, 25);
    generatedPasswordLineEdit->setReadOnly(true);
    generatedPasswordLineEdit->setFocusPolicy(Qt::StrongFocus);

    QVBoxLayout* layout = new QVBoxLayout(widget);
    widget->setLayout(layout);
    layout->addWidget(lengthPasswordLabel);
    layout->addWidget(spinBox);
    layout->addWidget(chooseLabel);
    layout->addWidget(numbersCheckBox);
    layout->addWidget(lettersCheckBox);
    layout->addWidget(specialCharactersCheckBox);
    layout->addWidget(generateButton);
    layout->addWidget(preGeneratedPasswordLabel);
    layout->addWidget(generatedPasswordLineEdit);

    QObject::connect(generateButton, &QPushButton::clicked, [spinBox, numbersCheckBox, lettersCheckBox,
                                                             specialCharactersCheckBox, generatedPasswordLineEdit]() {
        onGenerateClick(spinBox, numbersCheckBox, lettersCheckBox, specialCharactersCheckBox, generatedPasswordLineEdit);
    });

    widget->show();
    return a.exec();
}

void onGenerateClick(QSpinBox* spinBox, QCheckBox* numbersCheckBox, QCheckBox* lettersCheckBox,
                     QCheckBox* specialCharactersCheckBox, QLineEdit* generatedPasswordLineEdit) {
    generatedPasswordLineEdit->clear();

    int length = spinBox->value();
    bool useNumbers = numbersCheckBox->isChecked();
    bool useLetters = lettersCheckBox->isChecked();
    bool useSpecialCharacters = specialCharactersCheckBox->isChecked();

    if (!useNumbers && !useLetters && !useSpecialCharacters) {
        QMessageBox::warning(nullptr, "Error", "Please select at least one option");
        return;
    }

    QString possibleCharacters;

    if (useNumbers) {
        possibleCharacters += "0123456789";
    }
    if (useLetters) {
        possibleCharacters += "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (useSpecialCharacters) {
        possibleCharacters += "!@#$%^&*()_+-=[]{}|;:,.<>?";
    }

    // random_device - это генератор случайных чисел, обеспечивает генерацию случайных чисел
    // static означает, что переменная rd будет создана только один раз при первом вызове функции и сохранит своё состояние между вызовами
    // Это важно, чтобы каждый раз не создавать новый генератор, что может привести к повторяющимся результатам
    static std::random_device rd;
    // mt19937  - это генератор псевдослучайных чисел
    // static означает, что генератор generator будет создан только один раз и сохранит своё состояние между вызовами функции
    // Это важно для того, чтобы каждый вызов функции не начинал генерацию с одного и того же значения
    // Если бы генератор создавался каждый раз заново (без static), то при быстром нажатии на кнопку генерации пароля была бы генерация одинаковых паролей
    static std::mt19937 generator(rd());
    // uniform_int_distribution — это распределение случайных чисел, которое обеспечивает равномерное распределение целых чисел в заданном диапазоне
    // distribution(generator) — это вызов оператора, который генерирует случайное число в заданном диапазоне, используя генератор generator
    std::uniform_int_distribution<>distribution(0, possibleCharacters.size() - 1);

    QString password;
    for (int i = 0; i < length; i++) {
        password += possibleCharacters[distribution(generator)];
    }

    generatedPasswordLineEdit->setText(password);
}
