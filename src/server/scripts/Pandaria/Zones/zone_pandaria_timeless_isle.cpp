/*
 * Copyright (C) 2017-2018 AshamaneProject <https://github.com/AshamaneProject>
 * Copyright (C) 2016 Firestorm Servers <https://firestorm-servers.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "zone_pandaria_timeless_isle.h"

// Anduin Wrynn - 73061
class npc_prince_anduin : public CreatureScript
{
    public:
        npc_prince_anduin() : CreatureScript("npc_prince_anduin") {}

        struct npc_prince_anduinAI : public ScriptedAI
        {
            npc_prince_anduinAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;

            void DoAction(int32 action) override
            {
                switch (action)
                {
                    case ACTION_PRINCE_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_PRINCE_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_PRINCE_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_PRINCE_TALK_4:
                        Talk(3);
                        break;
                    case ACTION_PRINCE_TALK_5:
                        Talk(4);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 /*diff*/) override { }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_prince_anduinAI(creature);
        }
};

// Kairoz - 72870
class npc_kairoz : public CreatureScript
{
    public:
        npc_kairoz() : CreatureScript("npc_kairoz") {}

        bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
        {
            if (quest->GetQuestId() == QUEST_TIME_KEEPER_KAIROZ_H || quest->GetQuestId() == QUEST_TIME_KEEPER_KAIROZ_A)
            {
                if (creature->IsQuestGiver() && creature->AI())
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : ObjectGuid::Empty);
                    creature->AI()->DoAction(ACTION_CONVERSATION);
                }
            }
            return true;
        }

        struct npc_kairozAI : public ScriptedAI
        {
            npc_kairozAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap events;
            ObjectGuid playerGUID;
            bool conversationEngaged;

            void Reset() override
            {
                playerGUID = ObjectGuid::Empty;
                conversationEngaged = false;
            }

            void SetGUID(ObjectGuid guid, int32 /*id*/ /*= 0*/) override
            {
                playerGUID = guid;
            }

            void DoAction(int32 action) override
            {
                switch (action)
                {
                    case ACTION_KAIROZ_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_KAIROZ_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_KAIROZ_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_KAIROZ_TALK_4:
                        Talk(3);
                        break;
                    case ACTION_KAIROZ_TALK_5:
                        Talk(4);
                        break;
                    case ACTION_CONVERSATION:
                        if (!conversationEngaged)
                        {
                            conversationEngaged = true;
                            events.ScheduleEvent(EVENT_PRINCE_TALK_1, 3000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_1, 6000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_2, 11000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_2, 18000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_3, 25000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_3, 32000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_4, 39000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_4, 46000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_5, 54000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_5, 60000);
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 diff) override
            {
                events.Update(diff);

                std::list<Creature*> princeAnduinList;
                GetCreatureListWithEntryInGrid(princeAnduinList, me, NPC_PRINCE_ANDUIN, 7.0f);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_PRINCE_TALK_1:
                            if (!princeAnduinList.empty())
                                for (auto princeAnduin : princeAnduinList)
                                    princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_1);
                            break;
                        case EVENT_KAIROZ_TALK_1:
                            DoAction(ACTION_KAIROZ_TALK_1);
                            break;
                        case EVENT_PRINCE_TALK_2:
                            if (!princeAnduinList.empty())
                                for (auto princeAnduin : princeAnduinList)
                                    princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_2);
                            break;
                        case EVENT_KAIROZ_TALK_2:
                            DoAction(ACTION_KAIROZ_TALK_2);
                            break;
                        case EVENT_PRINCE_TALK_3:
                            if (!princeAnduinList.empty())
                                for (auto princeAnduin : princeAnduinList)
                                    princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_3);
                            break;
                        case EVENT_KAIROZ_TALK_3:
                            DoAction(ACTION_KAIROZ_TALK_3);
                            break;
                        case EVENT_PRINCE_TALK_4:
                            if (!princeAnduinList.empty())
                                for (auto princeAnduin : princeAnduinList)
                                    princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_4);
                            break;
                        case EVENT_KAIROZ_TALK_4:
                            DoAction(ACTION_KAIROZ_TALK_4);
                            break;
                        case EVENT_KAIROZ_TALK_5:
                            DoAction(ACTION_KAIROZ_TALK_5);
                            break;
                        case EVENT_PRINCE_TALK_5:
                            if (!princeAnduinList.empty())
                                for (auto princeAnduin : princeAnduinList)
                                {
                                    princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_5);
                                    conversationEngaged = false;
                                }
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_kairozAI(creature);
        }
};

class npc_emperor_shaohao : public CreatureScript
{
public:
    npc_emperor_shaohao() : CreatureScript("npc_emperor_shaohao") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_emperor_shaohaoAI(creature);
    }

    struct npc_emperor_shaohaoAI : public ScriptedAI
    {
        npc_emperor_shaohaoAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset()
        {
            me->setActive(true);
            events.Reset();
            _yulon = false;
            _xuen = false;
            _chiji = false;
            _niuzao = false;
            _challenge_started = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!_challenge_started && me->IsWithinDistInMap(who, 30.0f) && who->GetTypeId() == TYPEID_PLAYER && who->IsAlive())
            {
                _challenge_started = true;
                _yulon = true;
                _xuen = false;
                _chiji = false;
                _niuzao = false;
                me->AI()->Talk(EMPEROR_TALK_INTRO);
                events.ScheduleEvent(EVENT_EMPEROR_ARRANGE_BOSS, 16s);
            }
        }

        void DoAction(const int32 action) override
        {
            switch (action)
            {
                case ACTION_XUEN:
                    _xuen = true;
                    break;
                case ACTION_CHIJI:
                    _chiji = true;
                    break;
                case ACTION_NIUZAO:
                    _niuzao = true;
                    break;
                case ACTION_YULON:
                    _yulon = true;
                    break;
            }

            events.ScheduleEvent(EVENT_EMPEROR_ARRANGE_BOSS, 8s + 500ms);
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
                case EVENT_EMPEROR_ARRANGE_BOSS:
                {
                    if (_yulon)
                    {
                        if (Creature* yulon = me->FindNearestCreature(BOSS_YU_LON, 300.0f, true))
                        {
                            yulon->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_YULON);
                        }
                        else
                            _xuen = true;

                        _yulon = false;
                    }
                    if (_xuen)
                    {
                        if (Creature* xuen = me->FindNearestCreature(BOSS_XUEN, 300.0f, true))
                        {
                            xuen->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_XUEN);
                        }
                        else
                            _chiji = true;

                        _xuen = false;
                    }
                    if (_chiji)
                    {
                        if (Creature* chiji = me->FindNearestCreature(BOSS_CHI_JI, 300.0f, true))
                        {
                            chiji->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_CHIJI);
                        }
                        else
                            _niuzao = true;

                        _chiji = false;
                    }
                    if (_niuzao)
                    {
                        if (Creature* niuzao = me->FindNearestCreature(BOSS_NIUZAO, 300.0f, true))
                        {
                            niuzao->GetMotionMaster()->MovePoint(1, _timelessIsleMiddle);
                            me->AI()->Talk(EMPEROR_TALK_INTRO_NIUZAO);
                        }
                        else
                            _yulon = true;

                        _niuzao = false;
                    }

                    events.ScheduleEvent(EVENT_EMPEROR_ARRANGE_BOSS, 5s);
                    break;
                }
                default:
                    break;
            }
        }

        private:
            EventMap events;
            bool _yulon;
            bool _xuen;
            bool _chiji;
            bool _niuzao;
            bool _challenge_started;
    };
};

void AddSC_timeless_isle()
{
    new npc_prince_anduin();
    new npc_kairoz();
    new npc_emperor_shaohao();
}
