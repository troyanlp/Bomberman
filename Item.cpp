#include "Item.h"
#include "Application.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "MemLeaks.h"

Item::Item(iPoint pos, ItemType item) : itemType(item)
{
	LOG("Loading item");

	position = pos;

	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	collider->type = CITEM;
	
	type = EntityType::ITEM;

	graphics = App->textures->Load("Sprites/Enemies and items.gif");

	switch (item)
	{
	case IBOMB:
		animation.frames.push_back({ 188, 32, 16, 16 });
		animation.frames.push_back({ 188, 48, 16, 16 });
		collider->message = ColliderMessage::ITEMBOMB;
		break;
	case ILIFE:
		animation.frames.push_back({ 188, 96, 16, 16 });
		animation.frames.push_back({ 188, 112, 16, 16 });
		collider->message = ColliderMessage::ITEMLIFE;
		break;
	case ISPEED:
		animation.frames.push_back({ 204, 64, 16, 16 });
		animation.frames.push_back({ 204, 80, 16, 16 });
		collider->message = ColliderMessage::ITEMSPEED;
		break;
	case IFLAME:
		animation.frames.push_back({ 204, 32, 16, 16 });
		animation.frames.push_back({ 204, 48, 16, 16 });
		collider->message = ColliderMessage::ITEMFIRE;
		break;
	case IKILL:
		animation.frames.push_back({ 268, 32, 16, 16 });
		animation.frames.push_back({ 268, 48, 16, 16 });
		collider->message = ColliderMessage::ITEMDEATH;
		break;
	}
	animation.speed = 0.025;
	animation.loop = true;

	current_animation = &animation;

	spriteDest = { position.x, position.y, 50, 50 };

}


Item::~Item()
{
}

void Item::Draw()
{
	if (collider->collided && collider->message == ColliderMessage::NOTHING) {
		CleanUp();
	}

	if (!destroyed)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &spriteDest);
}

void Item::CleanUp()
{
	destroyed = true;
	collider->message = NOTHING;
	collider->to_delete = true;
	App->textures->Unload(graphics);
}
