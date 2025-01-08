declare type int = ( number | 0 );
declare type bool = boolean;


/**
 * 触发器原型
 */
declare type trigger = int;

/**
 * 游戏事件原型
 */

declare interface GameEvents{
    Events:{
        PosX:int,
        PosY:int,
        Image:int,
        Walkable:bool,
        Trigger:trigger;
        EventNodes:BasicEvent[]
    }[]
}

declare interface BasicEvent{
    EventCode:int,
    Flag:int
}