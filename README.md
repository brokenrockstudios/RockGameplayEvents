# RockGameplayEvents
A relatively simple event system for hooking up instanced actors and ability to broadcast events


With this plugin: you can hook up event listeners in the Unreal Engine's level editor. Between placed actors
Such as with a lightswitch and a light or a TriggerPad and a Door.

![image](https://github.com/user-attachments/assets/2f884fd9-c80d-48bf-a22c-229056318d52)

![image](https://github.com/user-attachments/assets/26adce9b-c585-4f92-8310-d895d2ffc467)


There are other great event systems out there that solve different issues.
Such as Lyra's GameplayMessageSubsystem or my modified version https://github.com/brokenrockstudios/GameplayMessageRouter

Also, there are many times where you likely just want to use a traditional `DECLARE_MULTICAST_DELEGATE` or some other built in.
But most all of these don't support hooking up 2 independent actors that are placed in a level without the usage of the Level Blueprint or other handcoded mechanisms.

Inspiration for this plugin came from UEFN's Direct Event Binding and I hope we can further expand and refine this particular solution.



