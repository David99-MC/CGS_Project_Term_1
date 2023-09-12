# RPG_AnimationSystem
# This project is used to further extend my gameplay programming knowledge in an RPG :) 
# Input instructions: 
- Movement: WASD
- Sprint: Shift
- Jump: Space bar
- Crouch: Ctrl
- Start/Stop climbing: F
- Attack: Left mouse button

# Door Problems:
- Used Dot Product to determine whether the Character is in front or behind the door.
- Used the combination of Timeline component and CurveFloat to smoothly rotate the door. They work great together as the Timeline component interpolates the Timeline progress (which is bound to with a call back) based on the CurveFloat (which is created in Unreal Editor).
- Pros:
  + Reduced the overall amount of code since I only need 1 variable for the door angle (Editable) and 1 variable determining if the door is closed or not.
- Cons:
  + The Forward Vector of the door has to be pointing in the same direction as the Character, otherwise use Right Vector if necessary.
  + The player must face in the same direction of the door for the Dot product to return correctly. More context: I used a BoxComponent to check when the Player has come in contact with the door, so while the Player is within the collision, they can face the other way which will make the Dot product return incorrectly.
==> *FIXED*: instead of applying the Dot product with the Forward Vector of both the Player and the door, calculate the Direction Vector between the Player and the door, THEN, apply Dot product with the Direction Vector and the door's Forward (Right if its coordinate is pointing wrong). The Direction Vector will correctly determine if the Player is in front or behind with regard to the door's Forward Vector.

# The Climbing Mechanics: 
- Please follow [this link](https://buulam.my.canva.site/contents-of-rpg-demo) to learn more in depth
