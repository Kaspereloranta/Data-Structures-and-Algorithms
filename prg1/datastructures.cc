// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    return places_.size();
}

void Datastructures::clear_all()
{
    places_.clear();
    areas_.clear();
}

std::vector<PlaceID> Datastructures::all_places()
{
    //TÄTÄ KANNATTAA YRITTÄÄ HIENOSÄÄTÄÄ TEHOKKAAMMAKSI, JOS JÄÄ AIKAA
    // TOISAALTA TÄMÄ EI OLE MUKANA TEHOKKUUSTESTEISSÄ.

    // voi ehkä miettiä std::transform() tähän, mutta se taitaa tuottaa myös
    // lineaarisen operaation

    std::vector<PlaceID> placeIDs;
    for(auto place : places_)
    {
        placeIDs.push_back(place.first); // push_back() is constant on complexity.
    }
    return placeIDs;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    Place newPlace = {name, type, xy};
    auto result = places_.insert(std::make_pair(id,newPlace)); // insert() complexity: average theta(1), worst-case: O(n).
                                                               // make_pair() complexity: constant. (theta(1)).

    // if places_ already include an element with same key (PlaceID), insert() does not add anything to places_.

    return result.second; // result.second is now a bool value which tells whether the insertion above was successfull or not.

}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{    

    std::unordered_map<PlaceID,Place>::const_iterator placeIt = places_.find(id); // Complexity of find(): Average: constant ( Theta(1))
                                                                                  // Worst case: Linear. O(n)
    if (placeIt == places_.end())
    {
        return {NO_NAME, PlaceType::NO_TYPE};
    }

    return std::make_pair(places_.at(id).placeName,places_.at(id).place); // .at() complexity: average constant, worst-case: O(n).
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    std::unordered_map<PlaceID,Place>::const_iterator placeIt = places_.find(id); // Complexity of find(): Average: constant ( Theta(1))
                                                                                  // Worst case: Linear. O(n).
    if (placeIt == places_.end())
    {
        return NO_COORD;
    }

    return places_.at(id).location; // .at() complexity: average constant (Theta(1)), worst-case: O(n).
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    // Replace this comment with your implementation
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    // Replace this comment with your implementation
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    // TÄMÄN TEHOKKUUTTA EHKÄ SYYTÄ HIOA VIELÄ

    std::map<Name,PlaceID> placesInAlphabeticalOrder;

   // for-loop complexity: O(n)
    for (auto place : places_)
    {
        placesInAlphabeticalOrder.insert({place.second.placeName,place.first}); // .insert() for map complexity: O(log n)
    }

    // now places are in alphabetical order since map sorts them.
    std::vector<PlaceID> placeIDsInOrder;

    // for loop complexity: O(n)
    for (auto place : placesInAlphabeticalOrder)
    {
        placeIDsInOrder.push_back(place.second); // push_back() is constant in time. O(1).
    }

    return placeIDsInOrder;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    // Let's use multimap to sort places based on their location and
    // store their PlaceIDs and y-coordinates as well for later inspection.
    std::multimap<double,std::pair<PlaceID,double>> places_in_order;
    // for-loop complexity: O(n)
    for (auto place : places_)
    {
        places_in_order.insert({sqrt(pow(place.second.location.x,2)+pow(place.second.location.y,2))
                                ,std::make_pair(place.first,place.second.location.y)});  // multiset.insert() complexity: O(log n)
    }

    // now places are in order based on their distance from origo in the multimap above
    // but places with the exactly same distance may be tumbled.

    std::vector<PlaceID> place_IDs_in_order;
    // let's add PlaceIDs to vector based on places' distance
    // and make sure that the places with same exact distance
    // are added to vector in the right order, in which the
    // place with the lowest y-coordinate will be added first.

    // for-loop complexity: O(n)
    auto itr = places_in_order.begin();
    ++itr;
    for (auto place : places_in_order)
    {
        if(place.first < itr->first and itr != places_in_order.end())
        {
            place_IDs_in_order.push_back(place.second.first);
        }
        else
        {
            if (itr == places_in_order.end())
            {
                place_IDs_in_order.push_back(place.second.first);
            }
            else if(place.second.second < itr->second.second)
            {
                place_IDs_in_order.push_back(place.second.first);
            }
            else if(itr->second.second < place.second.second)
            {
                place_IDs_in_order.push_back(itr->second.first);
            }
            else
            {
                place_IDs_in_order.push_back(place.second.first);
            }
        }
        ++itr;
    }

    // PALAA TÄHÄN VIELÄ, EI VÄLTTÄMÄTTÄ TOIMI HALUTULLA
    // TAVALLA YLEISESTI, MIKÄLI SAMAN ETÄISYYDEN OMAAVIA,
    // MUTTA LUKUISIA ERI Y-KOORDINAATIN PAIKKOJA ON MONIA.

    return place_IDs_in_order;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    // Replace this comment with your implementation
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}
