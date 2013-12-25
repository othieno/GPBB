/* Rotates and moves polygon-based objects (only balls, at the moment). */
#include "polygon.h"

#define X_BALL_MOVE 1.0
#define Y_BALL_MOVE 1.0
#define Z_BALL_MOVE 20.0

void RotateAndMoveBall(PObject * ObjectToMove)
{
   /* Change the spin axis from X to Y or Y to X if requested by the user */
   if (BallEvent & FLIP_SPIN_AXIS) {
      if (ObjectToMove->Rotate.RotateX != 0) {
         ObjectToMove->Rotate.RotateX = 0;
         ObjectToMove->Rotate.RotateY = 100; /* 10 degree Y rotations */
      } else if (ObjectToMove->Rotate.RotateY != 0) {
         ObjectToMove->Rotate.RotateY = 0;
         ObjectToMove->Rotate.RotateZ = 50; /* 5 degree Z rotations */
      } else {
         ObjectToMove->Rotate.RotateZ = 0;
         ObjectToMove->Rotate.RotateX = 75; /* 1.6 degree X rotations */
      }
      BallEvent &= ~FLIP_SPIN_AXIS;
   }

   /* Rotate the ball as needed */
   if (--ObjectToMove->RDelayCount == 0) {   /* rotate */
      ObjectToMove->RDelayCount = ObjectToMove->RDelayCountBase;
      if (ObjectToMove->Rotate.RotateX != 0)
         AppendRotationX(ObjectToMove->XformToWorld,
               ObjectToMove->Rotate.RotateX);
      if (ObjectToMove->Rotate.RotateY != 0)
         AppendRotationY(ObjectToMove->XformToWorld,
               ObjectToMove->Rotate.RotateY);
      if (ObjectToMove->Rotate.RotateZ != 0)
         AppendRotationZ(ObjectToMove->XformToWorld,
               ObjectToMove->Rotate.RotateZ);
      ObjectToMove->RecalcXform = 1;
   }

   /* Move the ball in response to recorded key events */
   if (BallEvent & MOVE_LEFT) {
      if (ObjectToMove->XformToWorld[0][3] > DOUBLE_TO_FIXED(-15000.0)) {
         ObjectToMove->XformToWorld[0][3] -= DOUBLE_TO_FIXED(X_BALL_MOVE);
         ObjectToMove->RecalcXform = 1;
      }
      BallEvent &= ~MOVE_LEFT;
   }
   if (BallEvent & MOVE_RIGHT) {
      if (ObjectToMove->XformToWorld[0][3] < DOUBLE_TO_FIXED(15000.0)) {
         ObjectToMove->XformToWorld[0][3] += DOUBLE_TO_FIXED(X_BALL_MOVE);
         ObjectToMove->RecalcXform = 1;
      }
      BallEvent &= ~MOVE_RIGHT;
   }
   if (BallEvent & MOVE_UP) {
      if (ObjectToMove->XformToWorld[1][3] < DOUBLE_TO_FIXED(15000.0)) {
         ObjectToMove->XformToWorld[1][3] += DOUBLE_TO_FIXED(Y_BALL_MOVE);
         ObjectToMove->RecalcXform = 1;
      }
      BallEvent &= ~MOVE_UP;
   }
   if (BallEvent & MOVE_DOWN) {
      if (ObjectToMove->XformToWorld[1][3] > DOUBLE_TO_FIXED(-15000.0)) {
         ObjectToMove->XformToWorld[1][3] -= DOUBLE_TO_FIXED(Y_BALL_MOVE);
         ObjectToMove->RecalcXform = 1;
      }
      BallEvent &= ~MOVE_DOWN;
   }
   if (BallEvent & MOVE_TOWARD) {
      if (ObjectToMove->XformToWorld[2][3] < DOUBLE_TO_FIXED(-100.0)) {
         ObjectToMove->XformToWorld[2][3] += DOUBLE_TO_FIXED(Z_BALL_MOVE);
         ObjectToMove->RecalcXform = 1;
      }
      BallEvent &= ~MOVE_TOWARD;
   }
   if (BallEvent & MOVE_AWAY) {
      if (ObjectToMove->XformToWorld[2][3] > DOUBLE_TO_FIXED(-15000.0)) {
         ObjectToMove->XformToWorld[2][3] -= DOUBLE_TO_FIXED(Z_BALL_MOVE);
         ObjectToMove->RecalcXform = 1;
      }
      BallEvent &= ~MOVE_AWAY;
   }
}

