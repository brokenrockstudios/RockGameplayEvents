# RockGameplayEvents
A relatively simple event system for hooking up instanced actors and ability to broadcast events

# The problem we are trying to solve
If you have ever wanted to connect the output event from 1 actor placed in the editor and trigger a function on another actor, but specifically hooking it up without the level blueprint and in the level editor.
Additionally without potential limitations of interfaces and/or components.

# Why you should use this plugin
If you want to connect an broadcastable event from 1 actor to another in the editor. 
If you want to accomplish this with ANY actor, no components and no interfaces.

Trigger: All you need is either an exposed RockGameplayEventDelegate variable in C++ or BP on the triggering actor
Receiver: All you need is a Function that matches the signature `void AnyNamedFunction(const AController*)` in either C++ or Blueprint. 

# How simple is it?
All you have to do is add a Broadcast Event when something happens

## C++
`OnHit.BroadcastEvent(someController);`
## Blueprint
![image](https://github.com/user-attachments/assets/19790a09-cb8a-405c-b154-24385d11c250)


To add at runtime

## C++
`OnHit.OnGameplayEvent.AddDynamic(this, &ABoxBase::SpinReceiver);`
## Blueprint
TBD



# How does it look in the editor?

With this plugin: you can hook up event listeners in the Unreal Engine's level editor. Between placed actors
Such as with a lightswitch and a light or a TriggerPad and a Door.

![image](https://github.com/user-attachments/assets/d0ceb039-39cc-4a02-a728-adb7eca77f6b)


There are other great event systems out there that solve different issues.
Such as Lyra's GameplayMessageSubsystem or our modified version of it https://github.com/brokenrockstudios/GameplayMessageRouter

Also, there are many times where you likely just want to use a traditional `DECLARE_MULTICAST_DELEGATE` or some other built in solutions.
However, most all of these don't support hooking up 2 independent actors that are placed in a level without the usage of the Level Blueprint or other handcoded mechanisms.


# License

Any portions of the code that are not covered by Epic's EULA are licensed under the MIT License, as specified in the LICENSE file.

- Unreal Engine EULA: https://www.unrealengine.com/eula
- Unreal Engine Content EULA: https://www.unrealengine.com/eula/content

# Credit

* Inspiration for this plugin came from UEFN's Direct Event Binding and I hope we can further expand and refine this particular solution.
* ben🍃ui's recent tutorial https://benui.ca/unreal/choosing-function-in-editor/


# Contribute!

I'd love to further refine or polish this off for a wider audience. I invite everyone to either request features or make suggestions or contribute via PR. 


