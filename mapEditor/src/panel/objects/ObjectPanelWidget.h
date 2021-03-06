#pragma once

#include <string>
#include <memory>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTabWidget>

#include <UrchinCommon.h>
#include <UrchinPhysicsEngine.h>
#include <controller/objects/ObjectController.h>
#include <panel/objects/ObjectTableView.h>
#include <panel/objects/bodyshape/BodyShapeWidget.h>

namespace urchin {

    class ObjectPanelWidget : public QWidget, public Observer, public Observable {
        Q_OBJECT

        public:
            ObjectPanelWidget();
            ~ObjectPanelWidget() override = default;

            enum NotificationType {
                OBJECT_BODY_SHAPE_WIDGET_CREATED
            };

            ObjectTableView* getObjectTableView() const;
            BodyShapeWidget* getBodyShapeWidget() const;

            void load(ObjectController&);
            void unload();

        private:
            void setupTransformBox(QVBoxLayout*);
            void setupPosition(QGridLayout*);
            void setupOrientation(QGridLayout*);
            void setupScale(QGridLayout*);
            void setupFlagsBox(QVBoxLayout*);
            void setupPhysicsBox(QVBoxLayout*);
            void setupPhysicsGeneralPropertiesBox(QVBoxLayout*);
            void setupPhysicsDampingPropertiesBox(QVBoxLayout*);
            void setupPhysicsFactorPropertiesBox(QVBoxLayout*);
            void setupPhysicsShapeBox(QVBoxLayout*);

            void notify(Observable*, int) override;
            void setupObjectDataFrom(const SceneObject*);
            void setupObjectPhysicsDataFrom(const SceneObject*);

            BodyShapeWidget* createBodyShapeWidget(const CollisionShape3D&, const SceneObject*);
            BodyShapeWidget* createNoBodyShapeWidget(const SceneObject*);
            void setupBodyShapeWidget();

            ObjectController* objectController;

            ObjectTableView* objectTableView;
            QPushButton* addObjectButton;
            QPushButton* removeObjectButton;
            QPushButton* cloneObjectButton;
            QTabWidget* tabWidget;

            bool disableObjectEvent;

            //general
            QDoubleSpinBox* positionX;
            QDoubleSpinBox* positionY;
            QDoubleSpinBox* positionZ;
            QComboBox* orientationType;
            QDoubleSpinBox* eulerAxis0;
            QDoubleSpinBox* eulerAxis1;
            QDoubleSpinBox* eulerAxis2;
            QDoubleSpinBox* scale;
            QCheckBox* produceShadowCheckBox;

            //physics
            QCheckBox* hasRigidBody;
            QTabWidget* tabPhysicsRigidBody;
            QVBoxLayout* physicsShapeLayout;
            QDoubleSpinBox* mass;
            QDoubleSpinBox* restitution;
            QDoubleSpinBox* friction;
            QDoubleSpinBox* rollingFriction;
            QDoubleSpinBox* linearDamping;
            QDoubleSpinBox* angularDamping;
            QDoubleSpinBox* linearFactorX;
            QDoubleSpinBox* linearFactorY;
            QDoubleSpinBox* linearFactorZ;
            QDoubleSpinBox* angularFactorX;
            QDoubleSpinBox* angularFactorY;
            QDoubleSpinBox* angularFactorZ;
            QLabel* shapeTypeValueLabel;
            QPushButton* changeBodyShapeButton;
            BodyShapeWidget* bodyShapeWidget;

        private slots:
            void showAddObjectDialog();
            void removeSelectedObject();
            void showCloneObjectDialog();

            void updateObjectOrientationType();
            void updateObjectTransform();
            void updateObjectScale();
            void updateObjectFlags();

            void showChangeBodyShapeDialog();
            void rigidBodyToggled(int);
            void updateObjectPhysicsProperties();
            void bodyShapeChanged(std::unique_ptr<const CollisionShape3D>&);
    };

}
